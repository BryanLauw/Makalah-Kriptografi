# Implementasi Steganografi Berbasis _Motion Vector_ pada Video H.264 (MP4)
> oleh Bryan Cornelius Lauwrence

## Daftar Konten
* [Informasi Umum](#informasi-umum)
* [Deskripsi Singkat](#deskripsi-singkat)
* [Kebutuhan Sistem](#kebutuhan-sistem)
* [Setup dan Penggunaan](#setup-dan-penggunaan)
* [Kreator](#kreator)

## Informasi Umum
Implementasi steganografi digital pada berkas video MP4 dengan skema *encoding* H.264 menggunakan manipulasi *motion vector*.

## Deskripsi Singkat
Steganografi digital merupakan teknik penyembunyian pesan rahasia ke dalam media digital lain tanpa mengubah persepsi visual secara signifikan.  
Pada media video MP4, metode LSB secara langsung kurang andal karena perbedaan struktur berkas dan proses *encoding*.

Pada proyek ini, pesan rahasia disisipkan ke dalam *motion vector* selama proses *encoding* H.264 menggunakan pustaka **x264**, sehingga pesan tersembunyi terintegrasi langsung ke dalam alur kompresi video.

## Kebutuhan Sistem
### Sistem Operasi
- Linux (Ubuntu 20.04 / 22.04 direkomendasikan)
> Pada sistem Windows, lingkungan Linux dapat dijalankan menggunakan **WSL (Ubuntu)**.
### Perangkat Lunak
- Bash shell
- GCC & Make
- FFmpeg
- NASM / YASM
- Python3

## Setup dan Penggunaan
1. Siapkan kebutuhan untuk menjalankan program
2. Clone repository ini dengan perintah `https://github.com/BryanLauw/Makalah-Kriptografi.git`
3. Pindah ke direktori utama dengan perintah `cd Makalah-Kriptografi`
4. Masuk ke wsl dengan perintah `wsl` (apabila menggunakan Windows)
5. Ubah perizinan file .sh dengan perintah `chmod +x build.sh setup.sh`
5. Jalankan `./setup.sh`
6. Jalankan `./build.sh`
7. Siapkan berkas MP4 untuk input pada folder `input/` (opsional)
7. Jika ingin menyisipkan, jalankan `./sisip` kemudian ikuti instruksi program
8. Jika ingin mengekstraksi, jalankan `venv/bin/python3 ./extract.py` kemudian ikuti instruksi program

## Kreator
| NIM | Nama |
|-----|------|
| 13522033 | Bryan Cornelius Lauwrence |