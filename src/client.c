#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "log.h"
#include "url_decoder.h"
#include "client.h"

void handle_client(int client_socket, struct sockaddr_in *client_addr) {
    char buffer[1024];
    char body[1024];
    int bytes_read;

    log_client_info(client_addr, "New client connected");

    bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from client");
        log_client_info(client_addr, "Error reading from client");
        close(client_socket);
        return;
    }
    buffer[bytes_read] = '\0';

    log_client_info(client_addr, "Client request received");

    char method[1024], path[1024];
    sscanf(buffer, "%s %s", method, path);

    char decoded_path[1024];
    url_decode(path, decoded_path);

    log_client_info(client_addr, "Processing client request");

    char response[1024];
    int response_length;

    if (strcmp(method, "GET") == 0) {
        response_length = snprintf(response, sizeof(response),
            "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 13\r\n\r\nGET received!");
    } else {
        response_length = snprintf(response, sizeof(response),
            "HTTP/1.1 405 Method Not Allowed\r\nContent-Type: text/plain\r\nContent-Length: 21\r\n\r\nMethod Not Allowed!");
    }

    write(client_socket, response, response_length);
    close(client_socket);
    log_client_info(client_addr, "Client connection closed");
}

