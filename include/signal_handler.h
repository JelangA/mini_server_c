#ifndef SIGNAL_HANDLER_H
#define SIGNAL_HANDLER_H

#include <signal.h>

// Variabel global untuk menghentikan server saat sinyal diterima
extern volatile sig_atomic_t keep_running;

// Fungsi untuk menangani sinyal (SIGINT atau SIGTERM)
// Parameter:
// - sig: jenis sinyal yang diterima
void signal_handler(int sig);

#endif // SIGNAL_HANDLER_H

