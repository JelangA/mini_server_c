#ifndef CHILD_H
#define CHILD_H

#include <netinet/in.h>

void child_process(int server_socket);
void spawn_child(int server_socket, int index);
void monitor_children(int server_socket);

#endif

