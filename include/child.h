#ifndef CHILD_H
#define CHILD_H

// Fungsi untuk membuat proses anak baru
// Parameter:
// - server_socket: socket server yang akan digunakan
// - child_id: ID proses anak
void spawn_child(int server_socket, int child_id);

// Fungsi untuk memantau proses anak
// Jika anak mati, fungsi ini akan membuat ulang proses anak
// Parameter:
// - server_socket: socket server yang akan digunakan
void monitor_children(int server_socket);

#endif // CHILD_H

