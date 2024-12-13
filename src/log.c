#include "log.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>

// File global untuk log
FILE *log_file = NULL;

// Fungsi untuk inisialisasi file log
void log_init(const char *filename) {
    log_file = fopen(filename, "a"); // Membuka file log dalam mode append
    if (!log_file) {
        perror("Failed to open log file");
        exit(EXIT_FAILURE);
    }
}

// Fungsi untuk menutup file log
void log_close() {
    if (log_file) {
        fclose(log_file); // Menutup file log
        log_file = NULL;
    }
}

// Fungsi untuk mencatat pesan ke file log
void log_message(const char *message) {
    if (log_file) {
        // Mendapatkan waktu saat ini untuk timestamp
        time_t now = time(NULL);
        char timestamp[20];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));

        // Menulis pesan ke file log dengan format [timestamp] [PID]
        fprintf(log_file, "[%s] [PID: %d] %s\n", timestamp, getpid(), message);
        fflush(log_file); // Memastikan data segera ditulis ke file
    }
}

