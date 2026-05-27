#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

// WIFI
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// ThingsBoard
const char* mqtt_server = "mqtt.thingsboard.cloud";
const char* token = "n9622l7uxbvblcsa1kxy";

// Pins
#define SOIL_PIN 34
#define WATER_PIN 35
#define LDR_PIN 32
#define PUMP_PIN 5
#define DHT_PIN 4
#define RAIN_PIN 33
#define PIR_PIN 19

#define DHTTYPE DHT22

WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHT_PIN, DHTTYPE);

// ================= STATE CONTROL =================
bool manualMode = false;   // false = AUTO, true = MANUAL
bool manualPump = false;   // dashboard pump control

int seuil = 40;

// ================= WIFI =================
void setup_wifi() {
  Serial.println("\nWiFi connecting...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
}

// ================= RPC CALLBACK =================
void callback(char* topic, byte* payload, unsigned int length) {
  String msg = "";

  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }

  Serial.println("RPC: " + msg);

  if (msg.indexOf("setMode") > 0) {
    manualMode = msg.indexOf("true") > 0;
    Serial.println(manualMode ? "MANUAL MODE" : "AUTO MODE");
  }

  if (msg.indexOf("setPump") > 0) {
    manualPump = msg.indexOf("true") > 0;
    Serial.println("Pump control received (manual mode)");
  }
}

// ================= MQTT RECONNECT =================
void reconnect() {
  while (!client.connected()) {
    String clientId = "ESP32-" + String(random(0xffff));

    if (client.connect(clientId.c_str(), token, "")) {
      Serial.println("MQTT connected ✔");
      client.subscribe("v1/devices/me/rpc/request/+");
    } else {
      Serial.print("MQTT failed, state=");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(PUMP_PIN, OUTPUT);
  pinMode(RAIN_PIN, INPUT_PULLUP);
  pinMode(PIR_PIN, INPUT);

  dht.begin();
  setup_wifi();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  randomSeed(micros());
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  // ================= SENSORS =================
  int soilRaw = analogRead(SOIL_PIN);
  int waterRaw = analogRead(WATER_PIN);
  int ldrRaw = analogRead(LDR_PIN);

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  bool rain = digitalRead(RAIN_PIN) == LOW;
  bool motion = digitalRead(PIR_PIN) == HIGH;

  int soil = map(soilRaw, 0, 4095, 0, 100);
  int water = map(waterRaw, 0, 4095, 0, 100);

  // 🔥 FIXED LDR (bright = high value)
  int ldr = map(ldrRaw, 0, 4095, 100, 0);

  if (isnan(temp) || isnan(hum)) {
    temp = 0;
    hum = 0;
  }

  // ================= PUMP LOGIC =================
  bool pumpState;

  if (manualMode) {
    pumpState = manualPump;
  } else {
    // 🌙 IMPROVED SMART LOGIC (optional upgrade included)
    bool isNight = ldr < 30;

    pumpState = (!rain && !motion && water > 20 && soil < seuil);

    // Optional safety rule:
    // if it's night + rain → force OFF pump
    if (rain && isNight) {
      pumpState = false;
    }
  }

  digitalWrite(PUMP_PIN, pumpState);

  // ================= TELEMETRY =================
  String payload = "{";
  payload += "\"soil\":" + String(soil) + ",";
  payload += "\"temperature\":" + String(temp) + ",";
  payload += "\"humidity\":" + String(hum) + ",";
  payload += "\"waterLevel\":" + String(water) + ",";
  payload += "\"ldr\":" + String(ldr) + ",";
  payload += "\"rain\":\"" + String(rain ? "Raining" : "No Rain") + "\",";
  payload += "\"motion\":\"" + String(motion ? "Motion Detected" : "No Motion") + "\",";
  payload += "\"pump\":" + String(pumpState ? 1 : 0) + ",";
  payload += "\"mode\":" + String(manualMode ? 1 : 0);
  payload += "}";

  client.publish("v1/devices/me/telemetry", payload.c_str());

  Serial.println(payload);

  delay(2000);
}