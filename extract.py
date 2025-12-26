import av
import sys
import os

def decode_message(bits):
    # Panjang pesan
    if len(bits) < 32: 
        return "Data terlalu pendek (kurang dari 32 bit)"
    
    # 32 bit pertama adalah panjang pesan
    length_val = 0
    for b in bits[:32]: 
        length_val = (length_val << 1) | b
    
    print(f"Panjang pesan rahasia: {length_val} karakter")
    
    if length_val <= 0: 
        return "Pesan gagal ditemukan."
    
    payload = bits[32:]
    message = ""
    
    for i in range(length_val):
        start = i * 8
        if start + 8 > len(payload): 
            break
        
        char_val = 0
        for b in payload[start:start+8]: 
            char_val = (char_val << 1) | b
            
        if 32 <= char_val <= 126:
            message += chr(char_val)
        else:
            message += f"{char_val}"
            
    return message

def load_map(map_file):
    """Membaca file peta koordinat"""
    targets = {}
    
    try:
        with open(map_file, 'r') as f:
            for line in f:
                parts = line.strip().split()
                if len(parts) == 3:
                    f_idx, mx, my = int(parts[0]), int(parts[1]), int(parts[2])
                    if f_idx not in targets:
                        targets[f_idx] = []
                    targets[f_idx].append((mx, my))
    except Exception as e:
        print(f"Gagal membaca file map: {e}")
        sys.exit(1)
    
    return targets

def main(video_file):    
    # 1. Baca kedua file (mp4 dan txt)
    base_name, _ = os.path.splitext(video_file)
    map_file = base_name + ".txt"

    print(f"Ekstraksi pesan rahasia dimulai")

    # Cek keberadaan file map
    if not os.path.exists(map_file):
        print(f"\nFile pemetaan hilang!")
        return

    # 2. Muat Peta
    target_map = load_map(map_file)
    
    # 3. Buka Video
    try:
        container = av.open(video_file)
    except Exception as e:
        print(f"Video rusak")
        return

    stream = container.streams.video[0]
    stream.codec_context.options = {'flags2': '+export_mvs'}
    stream.thread_type = "AUTO" 

    extracted_bits = []
    coding_frame_idx = 0

    print("Pencarian pesan rahasia...")

    for packet in container.demux(stream):
        for frame in packet.decode():
            
            if coding_frame_idx in target_map:
                frame_targets = target_map[coding_frame_idx]
                mvs = frame.side_data.get('MOTION_VECTORS')
                
                if mvs:
                    mv_lookup = {}
                    for mv in mvs:
                        if mv.w == 16 and mv.h == 16:
                            mb_x = int(mv.dst_x / 16)
                            mb_y = int(mv.dst_y / 16)
                            mv_lookup[(mb_x, mb_y)] = mv.motion_x
                    
                    for (target_x, target_y) in frame_targets:
                        if (target_x, target_y) in mv_lookup:
                            mx = mv_lookup[(target_x, target_y)]
                            # LSB
                            bit = int(mx) & 1
                            extracted_bits.append(bit)
                        else:
                            extracted_bits.append(0)
            
            coding_frame_idx += 1

    print(f"Pencarian selesai. Total bit ditemukan: {len(extracted_bits)}")
    
    if len(extracted_bits) > 0:
        msg = decode_message(extracted_bits)
        print("\nPesan Rahasia:")
        print(msg)
    else:
        print("Tidak ada bit yang terekstrak.")


if __name__ == "__main__":
    print("Ekstraksi pesan\n")

    loopInput = True
    while loopInput:
        video_file = input("Masukkan nama file stego (pastikan file ada di folder 'output/'): ")

        if not video_file.endswith(".mp4"):
            print("File harus berekstensi mp4")
            continue

        file_path = os.path.join("output", video_file)

        if not os.path.isfile(file_path):
            print("File tidak ditemukan di folder /output")
            continue

        loopInput = False

    main(file_path)