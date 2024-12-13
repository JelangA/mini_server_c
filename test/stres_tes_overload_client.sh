#!/bin/bash

# Jumlah koneksi yang ingin dibuat
jumlah_koneksi=100

# URL tujuan
url="http://localhost"

# Membuat 100 koneksi secara bersamaan
for i in $(seq 1 $jumlah_koneksi); do
  curl $url &  # Menjalankan curl di background untuk setiap koneksi
done

# Menunggu semua proses selesai
wait

