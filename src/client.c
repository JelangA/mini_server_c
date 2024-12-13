#include "client.h"
#include "log.h"
#include "url_decoder.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

// Fungsi untuk menangani klien
void handle_client(int client_socket, struct sockaddr_in *client_addr) {
    char buffer[BUFFER_SIZE];
    int bytes_read;

    log_message("New client connected");

    // Membaca permintaan dari klien
    bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from client");
        close(client_socket);
        return;
    }
    buffer[bytes_read] = '\0'; // Null-terminate buffer

    log_message("Client request received");

    // Memproses permintaan
    char method[BUFFER_SIZE], path[BUFFER_SIZE];
    sscanf(buffer, "%s %s", method, path);

    char decoded_path[BUFFER_SIZE];
    url_decode(path, decoded_path); // Decode URL

    log_message("Processing client request");

    // Mengirim respons
    char response[BUFFER_SIZE];
    int response_length = snprintf(response, sizeof(response),
        "HTTP/1.1 200 OK\r\nContent-Length: %ld\r\n\r\nHello, client!", strlen("Hello, client!"));
    write(client_socket, response, response_length);

    close(client_socket);
    log_message("Client connection closed");
}

