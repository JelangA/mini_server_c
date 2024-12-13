#include <time.h>
#include <unistd.h>
#include "log.h"

FILE *log_file = NULL;

void log_message(const char *message) {
    if (log_file) {
        time_t now = time(NULL);
        char timestamp[20];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
        fprintf(log_file, "[%s] [PID: %d] %s\n", timestamp, getpid(), message);
        fflush(log_file);
    }
}

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

