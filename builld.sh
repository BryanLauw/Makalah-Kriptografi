#!/bin/bash
set -e

echo "1. Compile x264"
cd x264-master
make -j4
cd ..

echo "2. Compile program C ---"
gcc main.c stego_core.c -o sisip

echo "Compile selesai"
echo "Jalankan dengan perintah: ./sisip"
