#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Ticker.H>


// Wi-Fi information
#define ssid "ESPtest"
#define password "MQTTtest2137!"
#define rxPin 4
#define txPin 5
#define baudRate 115200

// MQTT setup
char *mqtt_broker = "broker.emqx.io";
int mqtt_port = 1883;
char *mqtt_uname= "test";
char *mqtt_passwd= "test";
char *sendTopic= "esp/sendtopic";
char *reciveTopic= "esp/recivetopic";
char *gpsstatus= "esp/gpsstatus";


Ticker gpssend;


// constructors
WiFiClient wifiConnection;
PubSubClient client(wifiConnection);
TinyGPSPlus gps;
SoftwareSerial gpsSerial(rxPin, txPin);


// connect to wi-fi method
void wifi_connect(){ 
  WiFi.begin(ssid, password);             
  serial.print("connecting to WI-FI");
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500);
    serial.print(".").
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
   client.subscribe(reciveTopic);
   client.publish(sendTopic, "Hello there!");
}

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("new messeage in topic: ");
  Serial.println(reciveTopic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
      Serial.print((char) payload[i]);
  }
}

void checkGPS()
{
  if (gps.charsProcessed() < 10){
    client.publish(gpsstatus, "GPSError");
  }
  else{
    client.publish(gpsstatus,"GPSOK")
  }
}

void sendGPSData(){
   while (SerialGPS.available() >0) {
       gps.encode(SerialGPS.read());
    }
    client.publish(sats,gps.satellites.value());
    client.publish(lat,gps.location.lat()());
    client.publish(lng,gps.location.lng();());
    client.publish(day,gps.date.day());
    client.publish(month,gps.date.month());
    client.publish(year,gps.date.year());
    client.publish(hour,gps.time.hour());
    client.publish(minute,gps.time.minute());
    client.publish(second,gps.time.second());

}
   

void setup() {

  Serial.begin(baudRate);
  gpsSerial.begin(baudRate)
  client.setCallback(callback);
  wifi_connect();
  mqtt_connect();
  gpssend.attach(0.5,sendGPSData);
}

void loop() {
 client.loop();

}
