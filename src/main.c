#include "log.h"
#include "child.h"
#include "signal_handler.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    int server_socket;
    struct sockaddr_in server_addr;

    // Inisialisasi log
    log_init("server.log");
    log_message("Server starting");

    // Menangani sinyal SIGTERM dan SIGINT
    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);

    // Membuat socket server
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Failed to create socket");
        log_message("Error creating socket");
        log_close();
        exit(EXIT_FAILURE);
    }

    // Menyiapkan alamat server
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Mengikat socket ke alamat
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Failed to bind socket");
        log_message("Error binding socket");
        close(server_socket);
        log_close();
        exit(EXIT_FAILURE);
    }

    // Mulai mendengarkan koneksi
    if (listen(server_socket, BACKLOG) < 0) {
        perror("Failed to listen on socket");
        log_message("Error listening on socket");
        close(server_socket);
        log_close();
        exit(EXIT_FAILURE);
    }

    log_message("Server is listening");

    // Membuat proses anak
    for (int i = 0; i < NUM_CHILDREN; i++) {
        spawn_child(server_socket, i);
    }

    // Memantau proses anak
    monitor_children(server_socket);

    // Menutup server
    close(server_socket);
    log_message("Server shutting down");
    log_close();
    return 0;
}

