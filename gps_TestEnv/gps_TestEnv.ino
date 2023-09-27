#include <SPI.h>
#include <ESP8266WiFi.h>
#include <MQTT.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

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
bool enableGps = false;

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

int sessionID = 0 ;
void messageReceived(String &topic, String &payload ) {
  if (topic.compareTo("esp/detect") == 0 && payload.compareTo(WiFi.macAddress()) == 0) {
    Serial.println(F("lokalizuj"));
   digitalWrite(LED_BUILTIN, HIGH);
  } else if (topic.compareTo("gps/metric/enable") == 0) {
    enableGps = true;
    Serial.println(F("wllacz"));
  } else if (topic.compareTo("gps/metric/disable") == 0) {
    enableGps = false; 
    Serial.println(F("wylacz"));
  }
}
String convertGPSDateTime(String date, String time) {

  int year = date.substring(4, 6).toInt();
  int month = date.substring(2, 4).toInt();
  int day = date.substring(0, 2).toInt();
 date =  String(month < 10 ? "0" : "") + String(month) + "/" +
         String(day < 10 ? "0" : "") + String(day) + "/20" +
         String(year < 10 ? "0" : "") + String(year);

  int hour = time.substring(0, 2).toInt();
  int minute = time.substring(2, 4).toInt();
  int second = time.substring(4, 6).toInt();
   time = String(hour < 10 ? "0" : "") + String(hour) + ":" +
         String(minute < 10 ? "0" : "") + String(minute) + ":" +
         String(second < 10 ? "0" : "") + String(second);
  return date + " " + time;
}


void publishInfo()
{
String dateString = String(gps.date.value());
String timeString = String(gps.time.value());
Serial.print(timeString);
String datetime = convertGPSDateTime(dateString, timeString);
DynamicJsonDocument doc(1024);
doc["Device"] = String(WiFi.macAddress());
doc["Latitude"] = gps.location.lat();
doc["Longitude"] = gps.location.lng();
doc["Date"] = datetime;
String jsonStr;
  serializeJson(doc, jsonStr);
   Serial.print('\n');

client.publish("gps/metric", jsonStr);
serializeJson(doc, Serial);

}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  ss.begin(GPSBaud);
  Serial.begin(baudRate);
  WiFi.begin(ssid, pass);
  client.begin("192.168.0.123", net);
  connect();
}

void loop() {

  client.loop();
   
  client.onMessage(messageReceived);
  if (!client.connected()) {
    connect();
  }
 
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
    if (gps.location.isValid() && enableGps == true ){
      if (millis() - lastMillis > 3000) {
          publishInfo();
          lastMillis = millis();
    }
  }
}

