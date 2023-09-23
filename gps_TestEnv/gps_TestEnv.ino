#include <SPI.h>
#include <ESP8266WiFi.h>
#include <MQTT.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

#define RxPin 4
#define txPin 5
#define baudRate 115200
#define GPSBaud 9600
#define LED_BUILTIN 2

const char ssid[] = "Woz_operacyjny_ABW";
const char pass[] = "PiwkoPaliwko123!";



TinyGPSPlus gps;
SoftwareSerial ss(RxPin, txPin);
WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
   Serial.print(".");
  }

  Serial.print("\nconnecting...");
  while (!client.connect("arduino", "public", "public")) {
   Serial.print(".");
  }

  client.publish("esp/connection/connected", WiFi.macAddress());
  client.subscribe("esp/detect");
  client.subscribe("gps/metric/enable");
  client.subscribe("gps/metric/disable");
}

bool enableGps = true;
void messageReceived(String &topic, String &payload ) {

if (topic.compareTo("esp/detect") && payload.compareTo(WiFi.macAddress()) == 0){
    Serial.println(F("lokalizuj"));
} else if (topic.compareTo("gps/metric/enable") == 0){
  enableGps = true;
  Serial.println(F("wllacz"));
} else if (topic.compareTo("gps/metric/disable") == 0 ){
  enableGps = false; 
  Serial.println(F("wylacz"));
}

}

void setup() {
  ss.begin(GPSBaud);
  Serial.begin(baudRate);
  WiFi.begin(ssid, pass);
  client.begin("192.168.0.123", net);
  client.onMessage(messageReceived);

  connect();
}

void loop() {
  client.loop();

  if (!client.connected()) {
    connect();
  }
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      
    if (gps.location.isValid() && enableGps == true ){
      if (millis() - lastMillis > 5000) {
          client.publish("gps/" + WiFi.macAddress() + "/Latitude", String(gps.location.lat(),6));
          lastMillis = millis();
    }
  }
}
