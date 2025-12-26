/* stego_core.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "stego_core.h"

static StegoContext ctx = {NULL, 0, 0};

// Menyimpan pesan rahasia sementara
int stego_prepare_message_file(const char *filename, const char *secret_msg) {
    FILE *f = fopen(filename, "wb");
    if (!f) return 0;

    uint32_t len = (uint32_t)strlen(secret_msg);

    fputc((len >> 24) & 0xFF, f);
    fputc((len >> 16) & 0xFF, f);
    fputc((len >> 8)  & 0xFF, f);
    fputc((len)       & 0xFF, f);

    fwrite(secret_msg, 1, len, f);

    fclose(f);
    return 1;
}

// File ke memori
int stego_init_context(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) return 0;

    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    rewind(f);

    if (file_size == 0) { fclose(f); return 0; }

    ctx.data = (uint8_t*)malloc(file_size);
    if (!ctx.data) { fclose(f); return 0; }

    fread(ctx.data, 1, file_size, f);
    fclose(f);

    ctx.total_bits = file_size * 8;
    ctx.current_bit = 0;

    return 1;
}

// Iterasi bit pesan rahasia satu per satu
int stego_get_next_bit() {
    if (!ctx.data || ctx.current_bit >= ctx.total_bits) {
        return -1;
    }

    long byte_index = ctx.current_bit / 8;
    int bit_offset = 7 - (ctx.current_bit % 8);

    int bit = (ctx.data[byte_index] >> bit_offset) & 1;

    ctx.current_bit++;
    return bit;
}

// Membersihkan memori
void stego_cleanup() {
    if (ctx.data) {
        free(ctx.data);
        ctx.data = NULL;
    }
    ctx.total_bits = 0;
    ctx.current_bit = 0;
}

// Memastikan pesan rahasia masih ada atau tidak
bool stego_has_more_bits(){
    return ctx.current_bit >= ctx.total_bits;
}