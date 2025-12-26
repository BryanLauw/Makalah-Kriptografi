#!/bin/bash

# Hentikan script jika ada error
set -e

echo "Instalasi dependensi"

# 1. Update
echo "[1/4] Update package..."
sudo apt update

# 2. Install Assembler & FFmpeg Runtime
echo "[2/4] Install tools (GCC, Make), assembler & FFmpeg..."
sudo apt install -y build-essential git nasm yasm pkg-config ffmpeg

# 3. Install FFmpeg
echo "[3/4] Install FFmpeg development..."
sudo apt install -y libavcodec-dev libavformat-dev libavutil-dev libswscale-dev libavdevice-dev

# 4. Install Python
echo "[4/4] Install Python..."
sudo apt install -y python3 python3-pip python3-venv

# Buat Python Virtual Environment
echo "--> Membuat environment Python (venv)..."
if [ ! -d "venv" ]; then
    python3 -m venv venv
    echo "    Venv berhasil dibuat."
else
    echo "    Venv sudah ada."
fi

# Install Library av dan numpy
echo "--> Install 'av' and 'numpy' dengan pip..."
./venv/bin/pip install --upgrade pip
./venv/bin/pip install av numpy

echo "Instalasi selesai"
echo "-----------------------------------------------------------"
echo "untuk ektraksi jalankan script di bawah ini:"
echo ""
echo "    ./venv/bin/python3 extract.py <nama file mp4>"
echo ""
echo "-----------------------------------------------------------"