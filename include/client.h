#ifndef CLIENT_H
#define CLIENT_H

#include <netinet/in.h>

void handle_client(int client_socket, struct sockaddr_in *client_addr);

#endif

