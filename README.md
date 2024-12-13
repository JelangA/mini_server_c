# HTTP Server Project 🚀

Proyek ini mengimplementasikan server HTTP sederhana menggunakan bahasa C. Server ini mendukung fungsionalitas seperti routing, parsing parameter, dan penyajian file statis.

---

## Badges

![Language: C](https://img.shields.io/badge/language-C-00599C?style=for-the-badge&logo=c)
![Concurrency: Forking](https://img.shields.io/badge/Concurrency-Forking-00FF00?style=for-the-badge&logo=code)
![Makefile: Used](https://img.shields.io/badge/Makefile-Used-blue?style=for-the-badge&logo=makefile)
![WSL: Supported](https://img.shields.io/badge/WSL-Supported-blue?style=for-the-badge&logo=windows-subsystem-for-linux)
![HTTP Server: Active](https://img.shields.io/badge/HTTP%20Server-Active-blue?style=for-the-badge)
![TCP/IP: Supported](https://img.shields.io/badge/TCP%2FIP-Supported-lightblue?style=for-the-badge)
![Platform: Multi-OS](https://img.shields.io/badge/Platform-Linux%20%7C%20Windows%20%7C%20macOS-lightgray?style=for-the-badge)
![Dependencies: None](https://img.shields.io/badge/Dependencies-None-4B8B3B?style=for-the-badge)
![Azure Deployment: Deploying](https://img.shields.io/badge/Azure-Deploying-blue?style=for-the-badge&logo=azure)
![Unit Testing: Unity](https://img.shields.io/badge/Unit%20Testing-Unity-yellow?style=for-the-badge)

---

## Daftar Isi

1. [Visualisasi Repositori 📊](#visualisasi-repositori)
2. [Fitur 🌟](#fitur)
3. [Struktur File 📁](#struktur-file)
4. [Persiapan Awal 🛠️](#persiapan-awal)
    - [Prasyarat ⚙️](#prasyarat)
    - [Instalasi 💾](#instalasi)
5. [Penanganan Request HTTP 📡](#penanganan-request-http)
6. [Menjalankan Unit Test 🧪](#menjalankan-unit-test)
7. [Alur Program 🌀](#alur-program)
8. [Penulis ✍️](#penulis)

---

## Visualisasi Repositori 📊
Berikut adalah visualisasi repositori proyek ini.

---

## Fitur 🌟

- **Penanganan Request HTTP (GET, POST)**: Mendukung berbagai metode HTTP.
- **Concurrency dengan `fork`**: Mengelola proses anak untuk mendukung banyak koneksi.
- **Logging dan Debugging**: Menyediakan logging aktivitas server untuk memudahkan debugging.
- **Unit Testing**: Validasi fungsi server dengan unit testing.
- **Penanganan Zombie Process**: Memastikan proses zombie tidak muncul dengan pengelolaan sumber daya yang tepat.

---

## Struktur File 📁

- `server.c`: Titik masuk utama server. Bertanggung jawab untuk menangani koneksi klien, parsing request, dan dispatching request ke fungsi handler yang sesuai.
- Struktur folder:
  ![Struktur Folder](asset/struktur.jpg)

---

## Persiapan Awal 🛠️

### Prasyarat ⚙️

1. Windows Subsystem for Linux (WSL) (Opsional untuk pengguna Windows).
2. GCC Compiler.

### Instalasi 💾

1. **Install WSL (Opsional):**
    - [Panduan Install WSL](https://learn.microsoft.com/en-us/windows/wsl/install)

2. **Clone Repositori:**
    ```bash
    git clone https://github.com/username/repository.git
    cd repository
    ```

3. **Build Proyek:**
    ```bash
    make
    ```

4. **Jalankan Server:**
    ```bash
    ./server
    ```

---

## Penanganan Request HTTP 📡

Server ini mendukung pengujian metode HTTP menggunakan `curl` untuk mengirim request ke server yang berjalan.

### Metode GET
Contoh:
```bash
curl http://localhost:8081