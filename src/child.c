#include "child.h"
#include "log.h"
#include "client.h"
#include "signal_handler.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Array global untuk menyimpan PID dari proses anak
pid_t child_pids[NUM_CHILDREN];

// Fungsi untuk menangani proses anak
// Proses ini menerima koneksi klien dan memprosesnya
void child_process(int server_socket) {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    log_message("Child process started");

    while (keep_running) {
        // Menunggu koneksi dari klien
        int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket < 0) {
            if (errno == EINTR) {
                break; // Keluar jika sinyal diterima
            }
            perror("Failed to accept client connection");
            log_message("Error accepting client connection");
            continue;
        }

        // Tangani koneksi klien
        handle_client(client_socket, &client_addr);
    }

    log_message("Child process exiting");
}

// Fungsi untuk membuat proses anak
// Parameter:
// - server_socket: socket server yang akan digunakan
// - child_id: ID dari proses anak (indeks dalam array child_pids)
void spawn_child(int server_socket, int child_id) {
    pid_t pid = fork(); // Membuat proses anak
    if (pid < 0) {
        perror("Failed to fork");
        log_message("Error forking child process");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        // Proses anak menjalankan fungsi child_process
        child_process(server_socket);
        exit(EXIT_SUCCESS); // Proses anak selesai
    }
    // Simpan PID proses anak di array
    child_pids[child_id] = pid;
    log_message("Child process spawned");
}

// Fungsi untuk memantau proses anak
// Jika salah satu proses anak mati, fungsi ini akan membuat ulang proses anak
void monitor_children(int server_socket) {
    int status;
    while (keep_running) {
        pid_t pid = waitpid(-1, &status, WNOHANG); // Memeriksa proses anak tanpa menunggu
        if (pid > 0) {
            for (int i = 0; i < NUM_CHILDREN; i++) {
                if (child_pids[i] == pid) {
                    log_message("Child process crashed. Restarting...");
                    spawn_child(server_socket, i); // Membuat ulang proses anak
                    break;
                }
            }
        }
        sleep(1); // Hindari penggunaan CPU berlebihan
    }
}

