#include "signal_handler.h"

volatile sig_atomic_t keep_running = 1;

void signal_handler(int sig) {
    if (sig == SIGTERM || sig == SIGINT) {
        keep_running = 0;
    }
}

