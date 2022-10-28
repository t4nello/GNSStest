#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

//Hardware Setup
#define rxPin 4
#define txPin 5
#define baudRate 115200
#define GPSBaud 9600

// Wi-Fi information
#define ssid "ESPtest"
#define password "MQTTtest2137!"

//variables for delay without delay
unsigned long previousMillis = 0;
const long interval = 2000;  

char* toCharArray(String str) {
  return &str[0];
}

// Broker Setup
char *mqtt_broker = "192.168.0.123";
int mqtt_port = 8885;
char *mqtt_uname = "TestUser2";
char *mqtt_passwd = "test";

// topics about hardware info
char *gpsstart = "esp/gps/start";
char *brokerStatus = "esp/macaddress/";
char *wifissid = "esp/wifi/ssid";
char *gpsstatus= "esp/status/gps";


//GPS data topics 
          char *latTopic = "";
          char *lngTopic= "" ;
          char *spdTopic= "";
          char *dateTopic= ""; 
          char *timeTopic= ""; 
          char *satTopic= "";
          char *crsTopic= "";
          char *altTopic= ""; 

float latitude, longitude, speed,
  date, _time, satellites, course, altitude;

String lat_str, lng_str, spd_str, 
         date_str, time_str, sat_str, crs_str, alt_str;

char lat_ch[50] , lng_ch[50], spd_ch[50], 
          date_ch[50], time_ch[50], sat_ch[50], crs_ch[50], alt_ch[50];  

         
// constructors
WiFiClientSecure wifiConnection;
PubSubClient client(wifiConnection);
TinyGPSPlus gps;
SoftwareSerial gpsSerial(rxPin, txPin);

void brokerConnect(){
  WiFi.begin(ssid, password);        
  String client_id = "esp8266-client-";
    client_id += String(WiFi.macAddress());
    String topic1 = brokerStatus + String(WiFi.macAddress()) ;
    char ready[50]; 
    topic1.toCharArray(ready, topic1.length() + 1);  
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
   client.setServer(mqtt_broker, mqtt_port);
     while (!client.connected()) {
    if (client.connect(toCharArray(WiFi.macAddress()), mqtt_uname, mqtt_passwd)) {
    } else {
        Serial.print("failed with state ");
        Serial.print(client.state());
        delay(2000);
    }
        client.publish(ready, "connected" );
 } 
 }




String topics[10] = {"gps/lat/", "gps/lng/", "gps/spd/", "gps/date/", "gps/time/", "gps/sat/", "gps/crs/", "gps/alt/"};

char mac_ch[50];
String  mac = String(WiFi.macAddress());
void prepareTopics(){
for (int i=0; i<=7;i++){
    topics[i] += mac;
}}

void sendGPSData(){
          
    lat_str = String(gps.location.lat(),6); 
    lat_str.toCharArray(lat_ch, lat_str.length()+1); 
    topics[0].toCharArray(latTopic, topics[0].length()+1); 
    client.publish(latTopic,lat_ch); 

     lng_str = String(gps.location.lng(),6); 
     lng_str.toCharArray(lng_ch, lng_str.length() + 1); 
     topics[1].toCharArray(lngTopic, topics[1].length() + 1); 
     client.publish(lngTopic, lng_ch); 

     spd_str = String(gps.speed.kmph(),2); 
     spd_str.toCharArray(spd_ch, spd_str.length() + 1); 
     topics[2].toCharArray(spdTopic, topics[2].length() + 1); 
     client.publish(spdTopic, spd_ch); 

     date_str = String(gps.date.value()); 
     date_str.toCharArray(date_ch, date_str.length() + 1); 
     topics[3].toCharArray(dateTopic, topics[3].length() + 1); 
     client.publish(dateTopic, date_ch); 

     time_str = String(gps.time.value()); 
     time_str.toCharArray(time_ch, time_str.length() + 1); 
     topics[4].toCharArray(timeTopic, topics[4].length() + 1); 
     client.publish(timeTopic, time_ch); 

     sat_str = String(gps.satellites.value()); 
     sat_str.toCharArray(sat_ch, sat_str.length()+1); 
     topics[5].toCharArray(satTopic, topics[5].length() + 1); 
     client.publish(satTopic, sat_ch); 

     crs_str = String(gps.course.deg()); 
     crs_str.toCharArray(crs_ch, crs_str.length()+1); 
     topics[6].toCharArray(crsTopic, topics[6].length() + 1); 
     client.publish(crsTopic, crs_ch); 

     alt_str = String(gps.altitude.meters()); 
     alt_str.toCharArray(alt_ch, alt_str.length()+1); 
     topics[7].toCharArray(altTopic, topics[7].length() + 1); 
     client.publish(altTopic, alt_ch); 
}

String passValue= "";
String startGPS(char* topic, byte* message, unsigned int length) {

passValue.clear();
    for (int i = 0; i < length; i++) {
        Serial.print((char)message[i]);
        passValue += (char)message[i];
    }
    return passValue;
}
void setup() {
  gpsSerial.begin(GPSBaud);
  Serial.begin(baudRate);
  wifiConnection.setInsecure();
  brokerConnect();
  client.setCallback(startGPS);
  client.subscribe("esp/incoming");
  prepareTopics();
 }

void loop(){
unsigned long currentMillis = millis();
 client.loop();
 //Serial.print(passValue);
  while (gpsSerial.available() > 0 ) {
    if (gps.encode(gpsSerial.read()))
      {
        if (gps.location.isValid() && passValue == "on")
        {
          
           if (currentMillis - previousMillis >= interval  ) {
                previousMillis = currentMillis;
                sendGPSData();
     
              }
        }
      }
    }
}
