#include "signal_handler.h"
#include "log.h"

// Variabel global untuk menandakan apakah server tetap berjalan
volatile sig_atomic_t keep_running = 1;

// Fungsi untuk menangani sinyal (SIGTERM atau SIGINT)
void signal_handler(int sig) {
    if (sig == SIGTERM || sig == SIGINT) {
        log_message("Signal received, shutting down...");
        keep_running = 0; // Mengatur variabel untuk menghentikan server
    }
}

