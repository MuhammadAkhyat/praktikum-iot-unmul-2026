#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <DHT.h>

// WIFI
const char* WIFI_SSID = "Wokwi-GUEST";
const char* WIFI_PASSWORD = "";

// TELEGRAM BOT
const char* BOT_TOKEN = "8776111848:AAEl9WayK8RiA5Xl-zypxgQhpa6X2bQJvms";

String USER_A_ID = "5247143832";
String USER_B_ID = "8562386863";
String GROUP_CHAT_ID = "5247143832";

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

// PIN CONFIG
#define LED_A_PIN 18
#define LED_B_PIN 19
#define LED_UTAMA_PIN 21
#define DHTPIN 4
#define DHTTYPE DHT11
#define MQ2_PIN 34

DHT dht(DHTPIN, DHTTYPE);

// STATE
bool ledAState = false;
bool ledBState = false;
bool ledUtamaState = false;

const int GAS_THRESHOLD = 2000;
bool gasAlertSent = false;

unsigned long lastBotCheckTime = 0;
unsigned long lastSensorCheckTime = 0;

const unsigned long botInterval = 400;
const unsigned long sensorInterval = 1000;

// HELPER
bool isUserA(String userId) {
  return userId == USER_A_ID;
}

bool isUserB(String userId) {
  return userId == USER_B_ID;
}

bool canControlMainLed(String userId) {
  return isUserA(userId) || isUserB(userId);
}

String getLedText(bool state) {
  return state ? "ON ✅" : "OFF ❌";
}

String getGasStatus(int gasADC) {
  if (gasADC >= GAS_THRESHOLD) return "BAHAYA ";
  return "AMAN ";
}

String getClimateMessage() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    return " Gagal membaca sensor DHT11.\nPeriksa koneksi sensor.";
  }

  String msg = "🌡 *Kondisi Ruangan*\n";
  msg += "Suhu: " + String(t, 1) + " °C\n";
  msg += "Kelembapan: " + String(h, 1) + " %";
  return msg;
}

String getGasMessage() {
  int gasADC = analogRead(MQ2_PIN);

  String msg = " *Status Gas*\n";
  msg += "ADC MQ-2: " + String(gasADC) + "\n";
  msg += "Threshold: " + String(GAS_THRESHOLD) + "\n";
  msg += "Status: " + getGasStatus(gasADC) + "\n";
  msg += "_Catatan: di Wokwi gas disimulasikan sebagai gas level/ppm, sedangkan ESP32 membaca nilainya dalam bentuk ADC._";
  return msg;
}

String getStatusRumahMessage() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int gasADC = analogRead(MQ2_PIN);

  String msg = " *Status Rumah*\n";
  msg += "Lampu A: " + getLedText(ledAState) + "\n";
  msg += "Lampu B: " + getLedText(ledBState) + "\n";
  msg += "Lampu Utama: " + getLedText(ledUtamaState) + "\n";

  if (isnan(h) || isnan(t)) {
    msg += "Suhu: gagal dibaca\n";
    msg += "Kelembapan: gagal dibaca\n";
  } else {
    msg += "Suhu: " + String(t, 1) + " °C\n";
    msg += "Kelembapan: " + String(h, 1) + " %\n";
  }

  msg += "ADC Gas: " + String(gasADC) + "\n";
  msg += "Status Gas: " + getGasStatus(gasADC);

  return msg;
}

String getMenuMessage() {
  String msg = " *SMART HOUSE COMMAND LIST*\n\n";

  msg += " Informasi\n";
  msg += "/start - Mulai bot\n";
  msg += "/menu - Lihat semua command\n";
  msg += "/siapasaya - Cek identitas dan hak akses\n\n";

  msg += "🌡 Monitoring\n";
  msg += "/statusrumah - Status seluruh sistem\n";
  msg += "/cekiklim - Cek suhu & kelembapan\n";
  msg += "/cekgas - Cek kondisi gas\n\n";

  msg += " Lampu User A\n";
  msg += "/leda_on - Nyalakan Lampu A\n";
  msg += "/leda_off - Matikan Lampu A\n\n";

  msg += " Lampu User B\n";
  msg += "/ledb_on - Nyalakan Lampu B\n";
  msg += "/ledb_off - Matikan Lampu B\n\n";

  msg += " Lampu Utama\n";
  msg += "/ledutama_on - Nyalakan Lampu Utama\n";
  msg += "/ledutama_off - Matikan Lampu Utama\n\n";

  msg += " Catatan:\n";
  msg += "- User A hanya bisa Lampu A\n";
  msg += "- User B hanya bisa Lampu B\n";
  msg += "- Keduanya bisa Lampu Utama\n";
  msg += "- Monitoring sensor dapat diakses bersama";
  return msg;
}

String getUserRole(String userId) {
  if (isUserA(userId)) return "User A";
  if (isUserB(userId)) return "User B";
  return "Pengguna umum";
}

// HANDLE MESSAGE
void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id   = bot.messages[i].chat_id;
    String text      = bot.messages[i].text;
    String from_name = bot.messages[i].from_name;
    String from_id   = bot.messages[i].from_id;

    if (from_name == "") from_name = "User";

    Serial.println("PESAN MASUK");
    Serial.println("Chat ID : " + chat_id);
    Serial.println("From ID : " + from_id);
    Serial.println("Nama    : " + from_name);
    Serial.println("Text    : " + text);

    if (text == "/start") {
      String msg = " Selamat datang di *SMART HOUSE BOT*!\n";
      msg += "Halo, " + from_name + ".\n\n";
      msg += "Sistem rumah pintar siap digunakan.\n";
      msg += "Ketik /menu untuk melihat daftar perintah.";
      bot.sendMessage(chat_id, msg, "Markdown");
    }

    else if (text == "/menu") {
      bot.sendMessage(chat_id, getMenuMessage(), "Markdown");
    }

    else if (text == "/siapasaya") {
      String msg = " *Identitas Pengguna*\n";
      msg += "Nama: " + from_name + "\n";
      msg += "Role: " + getUserRole(from_id) + "\n";
      msg += "User ID: `" + from_id + "`\n\n";

      if (isUserA(from_id)) {
        msg += " Hak akses:\n";
        msg += "- Lampu A\n";
        msg += "- Lampu Utama\n";
        msg += "- Monitoring sensor";
      } else if (isUserB(from_id)) {
        msg += " Hak akses:\n";
        msg += "- Lampu B\n";
        msg += "- Lampu Utama\n";
        msg += "- Monitoring sensor";
      } else {
        msg += " Hak akses:\n";
        msg += "- Monitoring sensor\n";
        msg += "- Tidak memiliki akses lampu pribadi";
      }

      bot.sendMessage(chat_id, msg, "Markdown");
    }

    else if (text == "/cekiklim") {
      bot.sendMessage(chat_id, getClimateMessage(), "Markdown");
    }

    else if (text == "/cekgas") {
      bot.sendMessage(chat_id, getGasMessage(), "Markdown");
    }

    else if (text == "/statusrumah") {
      bot.sendMessage(chat_id, getStatusRumahMessage(), "Markdown");
    }

    else if (text == "/leda_on") {
      if (isUserA(from_id)) {
        digitalWrite(LED_A_PIN, HIGH);
        ledAState = true;
        bot.sendMessage(chat_id, " *AKSES DITERIMA!*\nLampu A berhasil *DINYALAKAN* oleh User A.", "Markdown");
      } else if (isUserB(from_id)) {
        bot.sendMessage(chat_id, " *AKSES DITOLAK!*\nUser B mencoba menyalakan Lampu A milik User A.", "Markdown");
      } else {
        bot.sendMessage(chat_id, " Kamu tidak memiliki akses untuk menyalakan Lampu A.", "Markdown");
      }
    }

    else if (text == "/leda_off") {
      if (isUserA(from_id)) {
        digitalWrite(LED_A_PIN, LOW);
        ledAState = false;
        bot.sendMessage(chat_id, " *AKSES DITERIMA!*\nLampu A berhasil *DIMATIKAN* oleh User A.", "Markdown");
      } else if (isUserB(from_id)) {
        bot.sendMessage(chat_id, " *AKSES DITOLAK!*\nUser B mencoba mematikan Lampu A milik User A.", "Markdown");
      } else {
        bot.sendMessage(chat_id, " Kamu tidak memiliki akses untuk mematikan Lampu A.", "Markdown");
      }
    }

    else if (text == "/ledb_on") {
      if (isUserB(from_id)) {
        digitalWrite(LED_B_PIN, HIGH);
        ledBState = true;
        bot.sendMessage(chat_id, " *AKSES DITERIMA!*\nLampu B berhasil *DINYALAKAN* oleh User B.", "Markdown");
      } else if (isUserA(from_id)) {
        bot.sendMessage(chat_id, " *AKSES DITOLAK!*\nUser A mencoba menyalakan Lampu B milik User B.", "Markdown");
      } else {
        bot.sendMessage(chat_id, " Kamu tidak memiliki akses untuk menyalakan Lampu B.", "Markdown");
      }
    }

    else if (text == "/ledb_off") {
      if (isUserB(from_id)) {
        digitalWrite(LED_B_PIN, LOW);
        ledBState = false;
        bot.sendMessage(chat_id, " *AKSES DITERIMA!*\nLampu B berhasil *DIMATIKAN* oleh User B.", "Markdown");
      } else if (isUserA(from_id)) {
        bot.sendMessage(chat_id, " *AKSES DITOLAK!*\nUser A mencoba mematikan Lampu B milik User B.", "Markdown");
      } else {
        bot.sendMessage(chat_id, " Kamu tidak memiliki akses untuk mematikan Lampu B.", "Markdown");
      }
    }

    else if (text == "/ledutama_on") {
      if (canControlMainLed(from_id)) {
        digitalWrite(LED_UTAMA_PIN, HIGH);
        ledUtamaState = true;
        bot.sendMessage(chat_id, " Lampu Utama Ruang Tamu berhasil *DINYALAKAN*.", "Markdown");
      } else {
        bot.sendMessage(chat_id, " Kamu tidak memiliki akses untuk mengontrol Lampu Utama.", "Markdown");
      }
    }

    else if (text == "/ledutama_off") {
      if (canControlMainLed(from_id)) {
        digitalWrite(LED_UTAMA_PIN, LOW);
        ledUtamaState = false;
        bot.sendMessage(chat_id, " Lampu Utama Ruang Tamu berhasil *DIMATIKAN*.", "Markdown");
      } else {
        bot.sendMessage(chat_id, " Kamu tidak memiliki akses untuk mengontrol Lampu Utama.", "Markdown");
      }
    }

    else {
      bot.sendMessage(chat_id, " Perintah tidak dikenali.\nKetik /menu untuk melihat daftar command.", "Markdown");
    }
  }
}

// GAS ALERT
void checkGasAlert() {
  int gasADC = analogRead(MQ2_PIN);

  Serial.print("MQ-2 ADC: ");
  Serial.println(gasADC);

  if (gasADC >= GAS_THRESHOLD && !gasAlertSent) {
    String msg = " *WARNING KEBOCORAN GAS!*\n";
    msg += "Nilai ADC MQ-2 melewati ambang batas aman.\n\n";
    msg += "ADC Gas: " + String(gasADC) + "\n";
    msg += "Threshold: " + String(GAS_THRESHOLD) + "\n";
    msg += "Status: *BAHAYA *\n";
    msg += "Segera lakukan pengecekan!";
    bot.sendMessage(GROUP_CHAT_ID, msg, "Markdown");
    gasAlertSent = true;
  }

  if (gasADC < GAS_THRESHOLD - 150) {
    gasAlertSent = false;
  }
}

// WIFI
void connectWiFi() {
  Serial.print("Menghubungkan WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi terhubung.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

// SETUP
void setup() {
  Serial.begin(115200);

  pinMode(LED_A_PIN, OUTPUT);
  pinMode(LED_B_PIN, OUTPUT);
  pinMode(LED_UTAMA_PIN, OUTPUT);

  digitalWrite(LED_A_PIN, LOW);
  digitalWrite(LED_B_PIN, LOW);
  digitalWrite(LED_UTAMA_PIN, LOW);

  dht.begin();

  connectWiFi();
  client.setInsecure();

  bot.sendMessage(GROUP_CHAT_ID, " SMART HOUSE BOT aktif dan siap digunakan.", "");
}

// LOOP
void loop() {
  if (millis() - lastBotCheckTime > botInterval) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    lastBotCheckTime = millis();
  }

  if (millis() - lastSensorCheckTime > sensorInterval) {
    checkGasAlert();
    lastSensorCheckTime = millis();
  }
}