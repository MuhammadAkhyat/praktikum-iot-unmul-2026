#define BLYNK_TEMPLATE_ID "TMPL69AUjiMvq"
#define BLYNK_TEMPLATE_NAME "Gas Monitor IoT"
#define BLYNK_AUTH_TOKEN "6z1ZgllvI-vB662niYFtKn76uomj_K0A"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>


// WiFi Credential
char ssid[] = "Wokwi-GUEST";
char pass[] = "";

#define MQ_PIN    34
#define LED1_PIN  18   // Hijau
#define LED2_PIN  19   // Kuning
#define LED3_PIN  21   // Merah

BlynkTimer timer;

// Variabel kontrol manual
int manualLED1 = 0;
int manualLED2 = 0;
int manualLED3 = 0;

// Variabel blink non-blocking
bool blinking = false;
int activeLED = 0;
unsigned long blinkStart = 0;
unsigned long lastBlink = 0;
bool ledState = false;
unsigned long blinkDuration = 0;

// Threshold
const int NO_GAS_THRESHOLD = 205;   // <= 205 dianggap "tidak terdeteksi"

// Fungsi bantu
void allLEDOff() {
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);
}

void startBlink(int ledPin, unsigned long durationMs) {
  blinking = true;
  activeLED = ledPin;
  blinkStart = millis();
  lastBlink = millis();
  blinkDuration = durationMs;
  ledState = false;

  allLEDOff();
}

void handleBlink() {
  if (!blinking) return;

  // toggle tiap 300 ms
  if (millis() - lastBlink >= 300) {
    lastBlink = millis();
    ledState = !ledState;
    digitalWrite(activeLED, ledState);
  }

  // stop jika durasi selesai
  if (millis() - blinkStart >= blinkDuration) {
    blinking = false;
    allLEDOff();
  }
}

void applyManualControl() {
  digitalWrite(LED1_PIN, manualLED1);
  digitalWrite(LED2_PIN, manualLED2);
  digitalWrite(LED3_PIN, manualLED3);
}

// Blynk kontrol LED manual
BLYNK_WRITE(V2) {
  manualLED1 = param.asInt();
}

BLYNK_WRITE(V3) {
  manualLED2 = param.asInt();
}

BLYNK_WRITE(V4) {
  manualLED3 = param.asInt();
}

// Kirim data sensor & logika
void sendSensorData() {
  int gasValue = analogRead(MQ_PIN); 

  // Supaya sesuai soal (0-500+), kita mapping ke 0-1000
  int gasLevel = map(gasValue, 0, 4095, 0, 1000);

  Serial.print("Raw ADC: ");
  Serial.print(gasValue);
  Serial.print(" | Gas Level: ");
  Serial.println(gasLevel);

  // kirim ke Blynk
  Blynk.virtualWrite(V0, gasLevel);

  // Jika gas tidak terdeteksi
  if (gasLevel <= NO_GAS_THRESHOLD) {
    Blynk.virtualWrite(V1, "Gas Tidak Terdeteksi - Mode Manual");


    if (!blinking) {
      applyManualControl();
    }
    return;
  }

  // Jika gas terdeteksi

  Blynk.virtualWrite(V1, "Gas Terdeteksi - Mode Otomatis");

  // Saat gas terdeteksi, abaikan kontrol manual
  manualLED1 = 0;
  manualLED2 = 0;
  manualLED3 = 0;

  Blynk.virtualWrite(V2, 0);
  Blynk.virtualWrite(V3, 0);
  Blynk.virtualWrite(V4, 0);

  // Jalankan blink hanya jika sedang tidak blinking
  if (!blinking) {
    if (gasLevel >= 0 && gasLevel <= 349) {
      startBlink(LED1_PIN, 3000); // 3 detik
    } 
    else if (gasLevel >= 350 && gasLevel <= 500) {
      startBlink(LED2_PIN, 5000); // 5 detik
    } 
    else if (gasLevel > 500) {
      startBlink(LED3_PIN, 7000); // 7 detik
    }
  }
}

// Setup
void setup() {
  Serial.begin(115200);

  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);

  allLEDOff();

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Timer baca sensor tiap 1 detik
  timer.setInterval(1000L, sendSensorData);
}

// Loop
void loop() {
  Blynk.run();
  timer.run();
  handleBlink();
}