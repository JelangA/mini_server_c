#include <sys/socket.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>
#include "child.h"
#include "log.h"
#include "client.h"

pid_t child_pids[4];

void child_process(int server_socket) {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    log_message("Child process started");

    while (1) {
        int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket < 0) {
            if (errno == EINTR) break;
            perror("Failed to accept client connection");
            continue;
        }
        handle_client(client_socket, &client_addr);
    }

    log_message("Child process exiting");
}

void spawn_child(int server_socket, int index) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("Failed to fork");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        child_process(server_socket);
        exit(EXIT_SUCCESS);
    }
    child_pids[index] = pid;
}

void monitor_children(int server_socket) {
    int status;
    while (1) {
        pid_t pid = waitpid(-1, &status, WNOHANG);
        if (pid > 0) {
            for (int i = 0; i < 4; i++) {
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

