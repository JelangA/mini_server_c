#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <time.h>
#include <ctype.h>
#include <arpa/inet.h>

#define PORT 8080
#define BACKLOG 10
#define NUM_CHILDREN 4
#define BUFFER_SIZE 1024

volatile sig_atomic_t keep_running = 1;
pid_t child_pids[NUM_CHILDREN];
FILE *log_file = NULL;

// Fungsi untuk menangani sinyal
void signal_handler(int sig) {
    if (sig == SIGTERM || sig == SIGINT) {
        keep_running = 0;
    }
}

// Fungsi untuk mencatat log dengan timestamp dan PID
void log_message(const char *message) {
    if (log_file) {
        time_t now = time(NULL);
        char timestamp[20];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
        fprintf(log_file, "[%s] [PID: %d] %s\n", timestamp, getpid(), message);
        fflush(log_file);
    }
}

// Fungsi untuk mencatat log dengan alamat klien
void log_client_info(struct sockaddr_in *client_addr, const char *message) {
    if (log_file) {
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr->sin_addr, client_ip, sizeof(client_ip));
        time_t now = time(NULL);
        char timestamp[20];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
        fprintf(log_file, "[%s] [PID: %d] [Client: %s:%d] %s\n", timestamp, getpid(), client_ip, ntohs(client_addr->sin_port), message);
        fflush(log_file);
    }
}

// Fungsi untuk decode URL
void url_decode(char *src, char *dest) {
    char a, b;
    while (*src) {
        if ((*src == '%') && ((a = src[1]) && (b = src[2])) && (isxdigit(a) && isxdigit(b))) {
            if (a >= 'a') a -= 'a' - 'A';
            if (a >= 'A') a -= ('A' - 10);
            else a -= '0';
            if (b >= 'a') b -= 'a' - 'A';
            if (b >= 'A') b -= ('A' - 10);
            else b -= '0';
            *dest++ = 16 * a + b;
            src += 3;
        } else if (*src == '+') {
            *dest++ = ' ';
            src++;
        } else {
            *dest++ = *src++;
        }
    }
    *dest = '\0';
}

// Fungsi untuk menangani klien
void handle_client(int client_socket, struct sockaddr_in *client_addr) {
    char buffer[BUFFER_SIZE];
    char body[BUFFER_SIZE];
    int bytes_read;

    log_client_info(client_addr, "New client connected");

    // Membaca permintaan dari klien
    bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from client");
        log_client_info(client_addr, "Error reading from client");
        close(client_socket);
        return;
    }
    buffer[bytes_read] = '\0'; // Null-terminate the buffer

    log_client_info(client_addr, "Client request received");
    printf("[DEBUG] Request: %s\n", buffer);

    // Ekstrak HTTP method dan path dari permintaan
    char method[BUFFER_SIZE], path[BUFFER_SIZE];
    sscanf(buffer, "%s %s", method, path);

    // Decode URL path
    char decoded_path[BUFFER_SIZE];
    url_decode(path, decoded_path);
    printf("[DEBUG] Decoded Path: %s\n", decoded_path);

    log_client_info(client_addr, "Processing client request");

    // Memisahkan header dan body dari permintaan HTTP
    char *body_ptr = strstr(buffer, "\r\n\r\n");
    if (body_ptr != NULL) {
        body_ptr += 4; // Mengabaikan \r\n\r\n
        strncpy(body, body_ptr, sizeof(body) - 1);
        body[sizeof(body) - 1] = '\0'; // Null-terminate the body
    } else {
        body[0] = '\0'; // Jika tidak ada body
    }

    printf("[DEBUG] Body: %s\n", body);

    char response[BUFFER_SIZE];
    int response_length;

    // Tangani berbagai metode HTTP
    if (strcmp(method, "GET") == 0) {
        log_client_info(client_addr, "Handling GET request");
        response_length = snprintf(response, sizeof(response),
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: %ld\r\n"
            "\r\n"
            "{\"message\": \"GET received!\"}", strlen("{\"message\": \"GET received!\"}"));
    } else if (strcmp(method, "POST") == 0) {
        log_client_info(client_addr, "Handling POST request");
        response_length = snprintf(response, sizeof(response),
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: %ld\r\n"
            "\r\n"
            "{\"message\": \"POST received!\", \"data\": \"%s\"}", strlen("{\"message\": \"POST received!\", \"data\": \"\"}") + strlen(body), body);
    } else if (strcmp(method, "PUT") == 0) {
        log_client_info(client_addr, "Handling PUT request");
        response_length = snprintf(response, sizeof(response),
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: %ld\r\n"
            "\r\n"
            "{\"message\": \"PUT received!\", \"data\": \"%s\"}", strlen("{\"message\": \"PUT received!\", \"data\": \"\"}") + strlen(body), body);
    } else if (strcmp(method, "DELETE") == 0) {
        log_client_info(client_addr, "Handling DELETE request");
        response_length = snprintf(response, sizeof(response),
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: %ld\r\n"
            "\r\n"
            "{\"message\": \"DELETE received!\", \"data\": \"%s\"}", strlen("{\"message\": \"DELETE received!\", \"data\": \"\"}") + strlen(body), body);
    } else {
        log_client_info(client_addr, "Unknown HTTP method");
        response_length = snprintf(response, sizeof(response),
            "HTTP/1.1 405 Method Not Allowed\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: %ld\r\n"
            "\r\n"
            "{\"message\": \"Method Not Allowed!\"}", strlen("{\"message\": \"Method Not Allowed!\"}"));
    }

    // Kirim respons ke klien
    if (write(client_socket, response, response_length) < 0) {
        perror("Failed to send response");
        log_client_info(client_addr, "Error sending response to client");
    } else {
        log_client_info(client_addr, "Response sent to client");
    }

    close(client_socket);
    log_client_info(client_addr, "Client connection closed");
}

// Fungsi untuk menangani proses anak
void child_process(int server_socket) {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    log_message("Child process started");

    while (keep_running) {
        int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket < 0) {
            if (errno == EINTR) {
                break;
            }
            perror("Failed to accept client connection");
            log_message("Error accepting client connection");
            continue;
        }

        struct timeval timeout;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
        setsockopt(client_socket, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));

        handle_client(client_socket, &client_addr);
    }

    log_message("Child process exiting");
}

// Fungsi untuk membuat proses anak
void spawn_child(int server_socket, int index) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("Failed to fork");
        log_message("Error forking child process");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        child_process(server_socket);
        exit(EXIT_SUCCESS);
    }
    child_pids[index] = pid;
    log_message("Child process spawned");
}

// Fungsi untuk memantau proses anak
void monitor_children(int server_socket) {
    int status;
    while (keep_running) {
        pid_t pid = waitpid(-1, &status, WNOHANG);
        if (pid > 0) {
            for (int i = 0; i < NUM_CHILDREN; i++) {
                if (child_pids[i] == pid) {
                    log_message("Child process crashed. Restarting...");
                    spawn_child(server_socket, i);
                    break;
                }
            }
        }
        sleep(1);
    }
}

// Fungsi utama
int main() {
    int server_socket;
    struct sockaddr_in server_addr;

    log_file = fopen("server.log", "a");
    if (!log_file) {
        perror("Failed to open log file");
        exit(EXIT_FAILURE);
    }

    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);

    log_message("Server starting");

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Failed to create socket");
        log_message("Error creating socket");
        fclose(log_file);
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Failed to bind socket");
        log_message("Error binding socket");
        close(server_socket);
        fclose(log_file);
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, BACKLOG) < 0) {
        perror("Failed to listen on socket");
        log_message("Error listening on socket");
        close(server_socket);
        fclose(log_file);
        exit(EXIT_FAILURE);
    }

    log_message("Server is listening");

    for (int i = 0; i < NUM_CHILDREN; i++) {
        spawn_child(server_socket, i);
    }

    monitor_children(server_socket);

    close(server_socket);
    log_message("Server shutting down");
    fclose(log_file);
    return 0;
}

