#  Smart Dam IoT (MQTT + Kodular)

##  Anggota Kelompok

1. Muhammad Akhyat Tariq R - NIM 2309106119


---

##  Judul Studi Kasus

**Miniatur Bendungan Pintar Berbasis IoT Menggunakan MQTT dan Kodular**

---

##  Deskripsi Project

Project ini merupakan sistem **miniatur bendungan pintar** yang mampu:

* Memonitor ketinggian air secara real-time
* Mengontrol pintu air secara otomatis maupun manual
* Memberikan peringatan menggunakan buzzer saat kondisi bahaya

Sistem menggunakan:

* **ESP32 (Wokwi)** sebagai controller
* **MQTT Broker** sebagai komunikasi data
* **Kodular** sebagai aplikasi monitoring dan kontrol

---

##  Fitur Utama

###  Monitoring

* Level ketinggian air
* Status kondisi (Aman / Waspada / Bahaya)
* Status servo (pintu air)
* Status buzzer
* Mode sistem (Auto / Manual)

###  Kontrol

* Mode otomatis & manual
* Kontrol servo (0°, 90°, 180°)
* Kontrol buzzer (ON/OFF)

---

##  Logika Sistem

| Kondisi | Nilai Sensor | Servo          | Buzzer   |
| ------- | ------------ | -------------- | -------- |
| AMAN    | ≤ 800        | 0° (tertutup)  | OFF      |
| WASPADA | 801 - 1500   | 90° (setengah) | OFF      |
| BAHAYA  | > 1500       | 180° (terbuka) | Berkedip |

---

##  MQTT Topic

###  Subscribe (Monitoring)

```
bendungan/sensor/levelair
bendungan/status/level
bendungan/status/servo
bendungan/status/buzzer
bendungan/status/mode
```

### 📤 Publish (Control)

```
bendungan/control/mode
bendungan/control/servo
bendungan/control/buzzer
```

---

##  Komponen yang Digunakan

### Hardware (Simulasi Wokwi)

* ESP32
* Potentiometer (sebagai pengganti water level sensor)
* Servo Motor
* Buzzer

### Software

* Wokwi Simulator
* Kodular
* MQTT Broker (broker.emqx.io)

---

##  Arsitektur Sistem

ESP32 (Wokwi)
  Publish data
MQTT Broker
  Subscribe
Kodular App

Kodular App
  Publish control
MQTT Broker
  Subscribe
ESP32

---

##  Konfigurasi MQTT

```
Broker : broker.emqx.io
Port   : 1883
```

---

##  Tampilan Aplikasi Kodular

Fitur yang ditampilkan:

* Level Air
* Status (warna indikator)
* Servo Position
* Buzzer Status
* Mode (AUTO / MANUAL)
* Tombol kontrol

---

##  Cara Menjalankan

### 1. Jalankan Wokwi

* Klik  Run
* Pastikan ESP32 terhubung ke MQTT

### 2. Jalankan Kodular

* Connect via Companion
* Pastikan MQTT terkoneksi

### 3. Uji Sistem

* Putar potentiometer → melihat perubahan level
* Tekan tombol → kontrol servo & buzzer

---

##  Pembagian Tugas

Muhammad Akhyat Tariq Razan : Rangkaian & Wokwi, Coding ESP32 & MQTT & Kodular & Dokumentasi

---

##  Board Schematic

<img width="708" height="545" alt="image" src="https://github.com/user-attachments/assets/9b4899df-c494-44de-911a-b3e6cd8c6cd7" />


