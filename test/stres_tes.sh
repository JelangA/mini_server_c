#!/bin/bash

# Jumlah klien yang akan diuji (Misalnya 1000 untuk lebih banyak koneksi)
NUM_CLIENTS=100

# File log untuk mencatat hasil
LOG_FILE="load_test_log.txt"
ERROR_LOG_FILE="load_test_error_log.txt"

# Tulis header log
echo "=== Mulai Pengujian Koneksi ===" > $LOG_FILE
echo "=== Mulai Pengujian Koneksi ===" > $ERROR_LOG_FILE

# Perintah untuk setiap klien
function send_request {
    # Catat waktu permintaan dimulai
    start_time=$(date +"%Y-%m-%d %H:%M:%S")
    
    # Kirim permintaan HTTP dengan netcat
    response=$(echo -e "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n" | nc -v -w 1 127.0.0.1 8080 2>&1)
    
    # Catat waktu permintaan selesai
    end_time=$(date +"%Y-%m-%d %H:%M:%S")
    
    # Simpan response ke log dan identifikasi apakah berhasil atau gagal
    if echo "$response" | grep -q "Connection refused"; then
        echo "$start_time - Gagal koneksi (Connection refused)" >> $ERROR_LOG_FILE
    else
        echo "$start_time - Permintaan berhasil - Response: $response - Selesai pada $end_time" >> $LOG_FILE
    fi
}

# Jalankan klien secara paralel
for i in $(seq 1 $NUM_CLIENTS); do
    send_request &  # Jalankan permintaan secara background
done

# Tunggu semua klien selesai
wait

# Tulis pesan selesai pengujian
echo "Pengujian selesai!" >> $LOG_FILE

