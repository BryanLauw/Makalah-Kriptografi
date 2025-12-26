#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "stego_core.h"

// Buffer
#define CMD_SIZE 4096
#define PATH_SIZE 256
#define MSG_SIZE 2048

// --- Helper Functions ---
// Menjalankan perintah shell/terminal
void run_command(const char *cmd) {
    printf("\n[SYSTEM CMD]: %s\n", cmd);
    int result = system(cmd);
    if (result != 0) {
        printf("[ERROR] Perintah gagal dijalankan (Exit Code: %d).\n", result);
    }
}

// Mengecek ada tidaknya file
int file_exists(const char *filename) {
    if (access(filename, F_OK) != -1) {
        return 1;
    } else {
        printf("[ERROR] File tidak ditemukan: %s\n", filename);
        return 0;
    }
}

void mode_sisip() {
    char input_filename[PATH_SIZE];
    char output_filename[PATH_SIZE];
    char input_path[PATH_SIZE];
    char output_path[PATH_SIZE];
    char map_output_path[PATH_SIZE];
    char secret_text[MSG_SIZE];
    char cmd[CMD_SIZE];

    // 1. Input Data
    printf("\nPenyisipan Pesan\n");
    printf("Pastikan file input ada di folder 'input/'\n");
    printf("Input nama file (misal: video.mp4): ");
    scanf("%255s", input_filename);

    sprintf(input_path, "input/%s", input_filename);
    if (!file_exists(input_path)) {
        printf("File tidak ditemukan!");
        return;
    };

    printf("Masukkan pesan rahasia: ");
    scanf(" %[^\n]", secret_text);

    printf("Nama file output (misal: stego.mp4): ");
    scanf("%255s", output_filename);
    sprintf(output_path, "output/%s", output_filename);

    // 2. Siapkan file pesan
    if (!stego_prepare_message_file(STEGO_MSG_FILE, secret_text)) {
        printf("Terjadi kesalahan: Gagal menulis file pesan.\n");
        return;
    }

    // Pastikan file map lama terhapus dulu agar tidak tercampur
    remove("stego_map.txt");

    printf("Memulai penyisipan...\n");
    // 3. Bangun Perintah Pipeline
    sprintf(cmd, 
        "ffmpeg -i %s -f yuv4mpegpipe - 2>/dev/null | "
        "./x264/x264 --demuxer y4m --threads 1 "
        "--bframes 0 --partitions none --no-dct-decimate "
        "--output intermediate.264 - ", 
        input_path);
    
    run_command(cmd);
    
    if (!file_exists("intermediate.264")) {
        printf("Encoding gagal, file intermediate.264 tidak terbentuk.\n");
        return;
    }

    // 4. Buat file mappint
    if (file_exists("stego_map.txt")) {
        // Buat nama file tujuan berdasarkan output_filename
        char base_name[PATH_SIZE];
        strcpy(base_name, output_filename);
        
        char *dot = strrchr(base_name, '.');
        if (dot) {
            *dot = '\0';
        }

        // Format nama file map: output/[nama_file].txt
        sprintf(map_output_path, "output/%s.txt", base_name);

        // Ubah nama
        if (rename("stego_map.txt", map_output_path) != 0) {
            printf("Gagal memindahkan stego_map.txt (Cek permission).\n");
        }
    } else {
        printf("stego_map.txt tidak ditemukan.\n");
    }

    sprintf(cmd, 
        "ffmpeg -i intermediate.264 -i %s -c:v copy -c:a copy -map 0:v -map 1:a -y %s 2>/dev/null", 
        input_path, output_path);
        
    run_command(cmd);

    // 5. Selesai
    printf("\nFile stego berhasil disimpan di: %s\n", output_path);
    
    remove("intermediate.264");
    remove(STEGO_MSG_FILE); 
}

int main() {
    int choice;
    
    while(1) {
        printf("\n==========================================\n");
        printf("   STEGANOGRAFI VIDEO H.264 (LSB MV)      \n");
        printf("==========================================\n");
        printf("1. Ya\n");
        printf("2. Tidak\n");
        printf("Apakah ingin menyisipkan?: ");
        
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n');
            choice = -1;
        }

        switch(choice) {
            case 1: mode_sisip(); break;
            case 2: 
                printf("Keluar program.\n");
                return 0;
            default: 
                printf("Pilihan tidak valid.\n");
        }
    }
    return 0;
}