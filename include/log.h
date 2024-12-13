#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <netinet/in.h>

extern FILE *log_file;

void log_message(const char *message);
void log_client_info(struct sockaddr_in *client_addr, const char *message);

#endif

