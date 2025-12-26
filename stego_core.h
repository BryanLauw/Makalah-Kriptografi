#ifndef STEGO_CORE_H
#define STEGO_CORE_H

#include <stdint.h>
#include <stdbool.h>

#define STEGO_MSG_FILE "stego_secret.dat"

// Struktur data bit di memori
typedef struct {
    uint8_t *data;      // Buffer byte
    long total_bits;    // Total bit (termasuk header panjang)
    long current_bit;   // Posisi cursor saat ini
} StegoContext;

// Mengubah string pesan menjadi format biner (Header Panjang + Pesan) dan simpan ke file
int stego_prepare_message_file(const char *filename, const char *secret_msg);

// Memuat file pesan ke memori
int stego_init_context(const char *filename);

// Mengambil bit selanjutnya untuk disisipkan (0 atau 1). Return -1 jika habis.
int stego_get_next_bit();

// Membersihkan memori
void stego_cleanup();

// Memastikan masih atau tidak adanhya bit untuk disisip
bool stego_has_more_bits();

#endif