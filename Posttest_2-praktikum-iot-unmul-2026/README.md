# Smart House Berbasis Telegram Bot

## Nama dan NIM
- Nama: Muhammad Akhyat Tariq Razan
- NIM: 2309106119

## Judul Studi Kasus
Smart House Berbasis Telegram Bot untuk Kontrol Lampu Pribadi, Lampu Utama, Monitoring Suhu/Kelembapan, dan Deteksi Kebocoran Gas

## Deskripsi
Project ini merupakan implementasi smart house berbasis Telegram Bot menggunakan ESP32. Sistem memiliki tiga lampu, yaitu Lampu A yang hanya dapat dikontrol oleh User A, Lampu B yang hanya dapat dikontrol oleh User B, dan Lampu Utama yang dapat dikontrol oleh keduanya. Sistem juga menggunakan sensor DHT11 untuk membaca suhu dan kelembapan serta sensor MQ-2 untuk mendeteksi kebocoran gas.

Jika kadar gas melebihi ambang batas aman, bot akan otomatis mengirim warning ke grup Telegram. Monitoring suhu, kelembapan, dan gas dapat diakses oleh pengguna.

## Fitur
- Kontrol Lampu A hanya untuk User A
- Kontrol Lampu B hanya untuk User B
- Kontrol Lampu Utama untuk User A dan User B
- Monitoring suhu
- Monitoring kelembapan
- Monitoring gas
- Warning otomatis ke grup Telegram saat gas berbahaya

## Pembagian Tugas
Project ini dikerjakan secara individu oleh:
- Muhammad Akhyat Tariq Razan : perancangan rangkaian, pembuatan program, integrasi Telegram Bot, pengujian, dokumentasi, dan video demo

## Komponen yang Digunakan
- ESP32
- 3 LED
- 3 resistor 220 ohm
- DHT11
- MQ-2
- Breadboard / Wokwi
- Kabel jumper
- WiFi
- Telegram Bot

## Wiring
- Lampu A -> GPIO 18
- Lampu B -> GPIO 19
- Lampu Utama -> GPIO 21
- DHT11 data -> GPIO 4
- MQ-2 analog -> GPIO 34

## List Command
- /start
- /menu
- /siapasaya
- /statusrumah
- /cekiklim
- /cekgas
- /leda_on
- /leda_off
- /ledb_on
- /ledb_off
- /ledutama_on
- /ledutama_off

## Cara Kerja Sistem
1. ESP32 terhubung ke WiFi
2. ESP32 terhubung ke Telegram Bot
3. Bot membaca command dari pengguna
4. Sistem memeriksa user ID pengirim
5. Jika command sesuai hak akses, lampu akan dikontrol
6. Jika command tidak sesuai hak akses, bot mengirim pesan akses ditolak
7. DHT11 membaca suhu dan kelembapan
8. MQ-2 membaca kondisi gas dalam bentuk ADC
9. Jika nilai ADC melewati threshold, bot mengirim warning otomatis ke grup

## Catatan Simulasi MQ-2
Pada Wokwi, sensor MQ-2 divisualisasikan dengan gas level / ppm. Namun pada program ESP32, data yang digunakan untuk pengambilan keputusan adalah nilai ADC hasil pembacaan analog. Oleh karena itu, status aman atau bahaya ditentukan berdasarkan threshold ADC.

## Rangkaian
<img width="888" height="615" alt="image" src="https://github.com/user-attachments/assets/65dbd844-d901-4b2a-8a9c-07dd6b0308e9" />

