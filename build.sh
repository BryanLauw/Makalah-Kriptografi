#!/bin/bash
set -e

echo "1. Build x264"

cd x264-master

chmod +x configure config.guess config.sub tools/*.sh

# configure
if [ ! -f config.h ]; then
    echo "Running configure..."
    ./configure
fi

make -j$(nproc)

cd ..

echo "2. Compile program C"

gcc embed.c stego_core.c -o sisip

echo "Build selesai"
echo "Jalankan dengan perintah: ./sisip"
