#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>



// Wi-Fi information
#define ssid "ESPtest"
#define password "MQTTtest2137!"
#define rxPin 4
#define txPin 5
#define baudRate 115200
#define GPSBaud 9600

// MQTT setup
char *mqtt_broker = "192.168.0.123";
int mqtt_port = 1883;
char *mqtt_uname= "TestUser1";
char *mqtt_passwd= "saper22";
char *receivetopic = "esp/incoming";
char *wifistatus= "esp/constatus";
char *gpsstatus= "esp/gpsstatus";
char *sats = "gps/satellites";
char *lat = "gps/lat";
char *lng = "gps/lng";
char *day = "gps/day";
char *month  ="gps/month";
char *year = "gps/year";
char *hour = "gps/hour";
char *minute = "gps/minte";
char *second = "gps/second";

float latitude , longitude;
String date_str , time_str , lat_str , lng_str;

// constructors
WiFiClient wifiConnection;
PubSubClient client(wifiConnection);
TinyGPSPlus gps;
SoftwareSerial gpsSerial(rxPin, txPin);


// connect to wi-fi method
void wifi_connect(){ 
  WiFi.begin(ssid, password);             
  Serial.print("connecting to WI-FI");
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to network with ip:");  
  Serial.println(WiFi.localIP());        
}

//connect to broker 
void mqtt_connect(){
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("No Wi-Fi connection, unable to connect to MQTT broker");
  }
   client.setServer(mqtt_broker, mqtt_port);
    if (client.connect("ESP8266", mqtt_uname, mqtt_passwd)) {
          Serial.println("Succesfully connected to broker ");
      } else {
          Serial.print("Connection failed");
      }
   client.publish(wifistatus, "ESP succesfully connected to Wi-FI");
}

// callback function to recieve messeages
void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("new messeage in topic: ");
  Serial.println(receivetopic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
      Serial.print((char) payload[i]);
  }
}
  void setup() {
    gpsSerial.begin(GPSBaud);
  Serial.begin(baudRate);

 
  client.setCallback(callback);
  wifi_connect();
  mqtt_connect();
  
}

String lat2String;
char lat[50];

void displayInfo() {
if (gps.location.isValid()) {
  lat2String = String(gps.location.lat(), 6);
  temp.toCharArray(lat, temp.length() + 1);
  client.publish(lat,lat);
}
}

void loop()
{
  while (gpsSerial.available() > 0){
    gps.encode(gpsSerial.read());
    displayInfo();
    delay(5000);
  }
   
  }
