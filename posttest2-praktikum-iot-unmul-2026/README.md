# POSTTEST 2 PRAKTIKUM IOT UNMUL 2026

## Monitoring Gas Level dan Kontrol LED Berbasis IoT Menggunakan ESP32, Sensor MQ-2, dan Blynk

---

## Nama dan NIM Anggota Kelompok
1. Muhammad Akhyat Tariq Razan - NIM 2309106119

---

## Deskripsi Proyek
Proyek ini merupakan implementasi sistem monitoring gas level berbasis Internet of Things (IoT) menggunakan board ESP32, sensor gas MQ-2, tiga buah LED, serta platform Blynk sebagai media monitoring dan kontrol.

Sistem dirancang untuk membaca level gas dari sensor MQ-2, kemudian memberikan indikator visual melalui tiga LED berdasarkan rentang nilai gas yang terdeteksi. Selain itu, sistem juga dapat dihubungkan ke platform IoT melalui koneksi Wi-Fi sehingga pengguna dapat memonitor nilai gas secara real-time dan mengontrol LED secara manual ketika gas tidak terdeteksi.

---

## Studi Kasus
Pada proyek ini, sistem dibuat untuk memenuhi ketentuan sebagai berikut:

1. Menggunakan 3 LED dan 1 sensor MQ (Gas).
2. Jika gas level berada pada rentang:
   - **0–349** maka **LED1** berkedip selama **3 detik**
   - **350–500** maka **LED2** berkedip selama **5 detik**
   - **>500** maka **LED3** berkedip selama **7 detik**
3. Jika gas tidak terdeteksi, maka **LED1, LED2, dan LED3 dapat dikontrol melalui platform IoT**.
4. Board yang digunakan adalah **ESP32**.
5. Sistem terhubung ke **Wi-Fi** dan **platform IoT**.

---

## Tujuan Proyek
Tujuan dari proyek ini adalah:

- Menerapkan konsep dasar Internet of Things (IoT) menggunakan ESP32.
- Mengintegrasikan sensor gas MQ-2 dengan mikrokontroler.
- Menggunakan LED sebagai indikator level gas.
- Menerapkan monitoring dan kontrol perangkat melalui platform Blynk.
- Memahami komunikasi data antara perangkat fisik dan dashboard IoT.

---

## Cara Kerja Sistem
Sistem bekerja dengan alur sebagai berikut:

1. Sensor MQ-2 membaca level gas di lingkungan.
2. ESP32 menerima data analog dari sensor melalui pin ADC.
3. Nilai sensor kemudian diproses menjadi **Gas Level** agar lebih mudah digunakan untuk logika sistem.
4. Berdasarkan nilai gas yang diperoleh:
   - Jika berada pada rentang **0–200**, maka **LED1** akan berkedip selama **3 detik**
   - Jika berada pada rentang **201–500**, maka **LED2** akan berkedip selama **5 detik**
   - Jika berada pada nilai **lebih dari 500**, maka **LED3** akan berkedip selama **7 detik**
5. Jika gas tidak terdeteksi, maka sistem akan masuk ke **mode manual**.
6. Pada mode manual, ketiga LED dapat dikontrol secara langsung melalui **dashboard Blynk**.
7. Nilai gas dan status sistem dikirimkan ke platform IoT untuk kebutuhan monitoring.

---

## Komponen yang Digunakan
Komponen yang digunakan pada proyek ini adalah:

- 1x ESP32 DevKit V1
- 1x Sensor Gas MQ-2
- 3x LED
  - LED1 (Hijau)
  - LED2 (Kuning)
  - LED3 (Merah)
- 3x Resistor 220Ω
- Platform Blynk IoT
- Koneksi Wi-Fi
- Simulasi menggunakan Wokwi

---

## Platform IoT yang Digunakan
Platform IoT yang digunakan pada proyek ini adalah **Blynk**.

Fungsi Blynk pada sistem ini adalah:
- Menampilkan nilai gas secara real-time
- Menampilkan status sistem
- Mengontrol LED1, LED2, dan LED3 saat mode manual
- Menjadi media komunikasi antara user dan ESP32 melalui internet

---

## Board Schematic

### Konfigurasi Pin
Berikut adalah konfigurasi pin yang digunakan:

- **MQ-2 AO** → **GPIO34**
- **LED1** → **GPIO18**
- **LED2** → **GPIO19**
- **LED3** → **GPIO21**

### Koneksi Komponen

#### Sensor MQ-2
- VCC → 5V ESP32
- GND → GND ESP32
- AO → GPIO34 ESP32

#### LED1
- Anoda (+) → Resistor 220Ω → GPIO18
- Katoda (-) → GND

#### LED2
- Anoda (+) → Resistor 220Ω → GPIO19
- Katoda (-) → GND

#### LED3
- Anoda (+) → Resistor 220Ω → GPIO21
- Katoda (-) → GND

---

## Tampilan Dashboard IoT
Dashboard Blynk pada proyek ini terdiri dari:

- **Gas Level** (untuk monitoring nilai gas)
- **Status Sistem** (untuk mengetahui mode sistem)
- **Switch LED1**
- **Switch LED2**
- **Switch LED3**

Dashboard ini digunakan untuk:
- memonitor nilai gas,
- melihat status sistem,
- dan mengontrol LED secara manual saat gas tidak terdeteksi.
<img width="1656" height="859" alt="image" src="https://github.com/user-attachments/assets/eabd0ee8-66fa-4290-8970-5b9ec4c66383" />

---

## Penjelasan Data Sensor
Dalam program, terdapat dua istilah utama yang digunakan:

### Raw ADC
Raw ADC adalah nilai mentah hasil pembacaan sensor gas oleh ESP32 melalui pin analog.

### Gas Level
Gas Level adalah nilai hasil pemetaan (mapping) dari Raw ADC agar lebih mudah digunakan dalam logika sistem dan ditampilkan pada dashboard.

Contoh:
- Raw ADC: nilai asli dari pembacaan analog sensor
- Gas Level: nilai yang digunakan untuk menentukan kategori kondisi gas

