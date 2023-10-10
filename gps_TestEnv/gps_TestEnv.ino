#include <SPI.h>
#include <ESP8266WiFi.h>
#include <MQTT.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <TimeLib.h>

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
  while (!client.connect((WiFi.macAddress().c_str()), "public", "public")) {
   Serial.print(".");
  }

  client.publish("esp/connection/connected", WiFi.macAddress());
  client.subscribe("esp/detect");
  client.subscribe("gps/metric/enable");
  client.subscribe("gps/metric/disable");
}

int SessionId=0;
void messageReceived(String &topic, String &payload) {

  if (topic.compareTo("esp/detect") == 0 && payload.compareTo(WiFi.macAddress()) == 0) {
  
    digitalWrite(LED_BUILTIN, HIGH);
  } else if (topic.compareTo("gps/metric/enable") == 0) {
    SessionId = payload.toInt();
    if (SessionId == 0){
    enableGps = false;
    }
    else
    enableGps = true;
    
   
  } else if (topic.compareTo("gps/metric/disable") == 0) {
    enableGps = false;
  }
}

long convertGPSDateTime() {
        int year = gps.date.year();
        int month = gps.date.month();
        int day = gps.date.day();
        int hour = gps.time.hour();
        int minute = gps.time.minute();
        int second = gps.time.second();

       
        tmElements_t tm;
        tm.Year = year - 1970; 
        tm.Month = month;
        tm.Day = day;
        tm.Hour = hour;
        tm.Minute = minute;
        tm.Second = second;
        time_t timestamp = makeTime(tm);
     return timestamp;
}


void publishInfo()
{
String dateString = String(gps.date.value());
String timeString = String(gps.time.value());
long datetime = convertGPSDateTime();
StaticJsonDocument<1024> doc;
doc["device"] = WiFi.macAddress();
doc["latitude"] = gps.location.lat();
doc["longitude"] = gps.location.lng();
doc["speed"] = gps.speed.kmph();
doc["altitude"] = gps.altitude.meters();
doc["satellites"] = gps.satellites.value();
doc["time"] = datetime;
doc["sessionid"] = SessionId;
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

