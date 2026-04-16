#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "broker.emqx.io";  
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);
Servo myServo;

// Pin Wokwi
const int waterSensorPin = 34;  // pakai potentiometer sebagai simulasi water level
const int servoPin = 18;
const int buzzerPin = 23;

// Topic MQTT
const char* topic_levelair    = "bendungan/sensor/levelair";
const char* topic_statuslevel = "bendungan/status/level";
const char* topic_statusservo = "bendungan/status/servo";
const char* topic_statusbuzzer= "bendungan/status/buzzer";
const char* topic_statusmode  = "bendungan/status/mode";

const char* topic_ctrl_mode   = "bendungan/control/mode";
const char* topic_ctrl_servo  = "bendungan/control/servo";
const char* topic_ctrl_buzzer = "bendungan/control/buzzer";

// Variabel sistem
String modeSystem = "AUTO";
String statusLevel = "AMAN";
String buzzerStatus = "OFF";
int servoAngle = 0;
int waterValue = 0;

// Kontrol manual
int manualServo = 0;
String manualBuzzer = "OFF";

// Kedip buzzer saat bahaya
unsigned long previousMillis = 0;
const long blinkInterval = 500;
bool buzzerBlinkState = false;

void setup_wifi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  String topicStr = String(topic);
  Serial.print("Message [");
  Serial.print(topicStr);
  Serial.print("] = ");
  Serial.println(message);

  if (topicStr == topic_ctrl_mode) {
    if (message == "AUTO" || message == "MANUAL") {
      modeSystem = message;
    }
  }

  if (modeSystem == "MANUAL") {
    if (topicStr == topic_ctrl_servo) {
      manualServo = message.toInt();
      if (manualServo < 0) manualServo = 0;
      if (manualServo > 180) manualServo = 180;
    }

    if (topicStr == topic_ctrl_buzzer) {
      if (message == "ON" || message == "OFF") {
        manualBuzzer = message;
      }
    }
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting MQTT...");
    String clientId = "wokwi-bendungan-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe(topic_ctrl_mode);
      client.subscribe(topic_ctrl_servo);
      client.subscribe(topic_ctrl_buzzer);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retry 2s");
      delay(2000);
    }
  }
}

void publishStatus() {
  client.publish(topic_levelair, String(waterValue).c_str(), true);
  client.publish(topic_statuslevel, statusLevel.c_str(), true);
  client.publish(topic_statusservo, String(servoAngle).c_str(), true);
  client.publish(topic_statusbuzzer, buzzerStatus.c_str(), true);
  client.publish(topic_statusmode, modeSystem.c_str(), true);
}

void handleAutoMode() {
  if (waterValue <= 800) {
    statusLevel = "AMAN";
    servoAngle = 0;
    buzzerStatus = "OFF";
    myServo.write(servoAngle);
    digitalWrite(buzzerPin, LOW);
  } 
  else if (waterValue <= 1500) {
    statusLevel = "WASPADA";
    servoAngle = 90;
    buzzerStatus = "OFF";
    myServo.write(servoAngle);
    digitalWrite(buzzerPin, LOW);
  } 
  else {
    statusLevel = "BAHAYA";
    servoAngle = 180;
    myServo.write(servoAngle);

    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= blinkInterval) {
      previousMillis = currentMillis;
      buzzerBlinkState = !buzzerBlinkState;
      digitalWrite(buzzerPin, buzzerBlinkState);
    }

    buzzerStatus = buzzerBlinkState ? "ON" : "OFF";
  }
}

void handleManualMode() {
  // status tetap berdasarkan sensor
  if (waterValue <= 800) {
    statusLevel = "AMAN";
  } else if (waterValue <= 1500) {
    statusLevel = "WASPADA";
  } else {
    statusLevel = "BAHAYA";
  }

  // aktuator berdasarkan app
  servoAngle = manualServo;
  myServo.write(servoAngle);

  if (manualBuzzer == "ON") {
    digitalWrite(buzzerPin, HIGH);
    buzzerStatus = "ON";
  } else {
    digitalWrite(buzzerPin, LOW);
    buzzerStatus = "OFF";
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  myServo.attach(servoPin);
  myServo.write(0);

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  waterValue = analogRead(waterSensorPin);
  Serial.print("Water Level: ");
  Serial.println(waterValue);

  if (modeSystem == "AUTO") {
    handleAutoMode();
  } else {
    handleManualMode();
  }

  publishStatus();
  delay(1000);
}