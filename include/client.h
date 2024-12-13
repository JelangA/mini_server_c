#ifndef CLIENT_H
#define CLIENT_H

#include <netinet/in.h>

// Fungsi untuk menangani permintaan klien
// Parameter:
// - client_socket: socket klien yang terhubung
// - client_addr: informasi alamat klien
void handle_client(int client_socket, struct sockaddr_in *client_addr);

#endif // CLIENT_H

