#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "log.h"
#include "signal_handler.h"
#include "child.h"

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
        fclose(log_file);
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Failed to bind socket");
        close(server_socket);
        fclose(log_file);
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 10) < 0) {
        perror("Failed to listen on socket");
        close(server_socket);
        fclose(log_file);
        exit(EXIT_FAILURE);
    }

    log_message("Server is listening");

    for (int i = 0; i < 4; i++) {
        spawn_child(server_socket, i);
    }

    monitor_children(server_socket);

    close(server_socket);
    log_message("Server shutting down");
    fclose(log_file);
    return 0;
}

