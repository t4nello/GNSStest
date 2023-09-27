#include <SPI.h>
#include <ESP8266WiFi.h>
#include <MQTT.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

#undef  MQTT_MAX_PACKET_SIZE 
#define MQTT_MAX_PACKET_SIZE 500 
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
MQTTClient client(256);

unsigned long lastMillis = 0;
bool enableGps = false;

void connect() {
  
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
void messageReceived(String &topic, String &payload) {
  if (topic.compareTo("esp/detect") == 0 && payload.compareTo(WiFi.macAddress()) == 0) {
  
    digitalWrite(LED_BUILTIN, HIGH);
  } else if (topic.compareTo("gps/metric/enable") == 0) {
    sessionID = payload.toInt();
    enableGps = true;
    
   
  } else if (topic.compareTo("gps/metric/disable") == 0) {
    enableGps = false;
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
String datetime = convertGPSDateTime(dateString, timeString);
StaticJsonDocument<1024> doc;
doc["Device"] = WiFi.macAddress();
doc["Latitude"] = gps.location.lat();
doc["Longitude"] = gps.location.lng();
doc["Date"] = datetime;
doc["SessionID"] = sessionID;
 String jsonStr;
  serializeJson(doc, jsonStr);
 
  client.publish("gps/metric", jsonStr);
 

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
  delay(10);
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

