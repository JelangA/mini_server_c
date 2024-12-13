#include "url_decoder.h"
#include <ctype.h>

// Fungsi untuk mendecode URL
void url_decode(const char *src, char *dest) {
    char a, b;
    while (*src) {
        if ((*src == '%') && ((a = src[1]) && (b = src[2])) && (isxdigit(a) && isxdigit(b))) {
            // Decode karakter hex menjadi karakter ASCII
            if (a >= 'a') a -= 'a' - 'A';
            if (a >= 'A') a -= ('A' - 10);
            else a -= '0';
            if (b >= 'a') b -= 'a' - 'A';
            if (b >= 'A') b -= ('A' - 10);
            else b -= '0';
            *dest++ = 16 * a + b;
            src += 3;
        } else if (*src == '+') {
            *dest++ = ' '; // '+' menjadi spasi
            src++;
        } else {
            *dest++ = *src++; // Salin karakter lain
        }
    }
    *dest = '\0'; // Null-terminate string
}

