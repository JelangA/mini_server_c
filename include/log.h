#ifndef LOG_H
#define LOG_H

// Inisialisasi file log
// Parameter:
// - filename: nama file log yang akan digunakan
void log_init(const char *filename);

// Menutup file log
void log_close();

// Menulis pesan ke file log dengan format timestamp dan PID
// Parameter:
// - message: pesan yang akan dicatat di log
void log_message(const char *message);

#endif // LOG_H

