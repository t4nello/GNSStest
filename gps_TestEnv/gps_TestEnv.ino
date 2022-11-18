#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

//Hardware Setup
#define rxPin 4
#define txPin 5
#define baudRate 115200
#define GPSBaud 9600
#define LED_BUILTIN 2

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
char *locate= "esp/locate";


String  mac = String(WiFi.macAddress());


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
          date_ch[50], time_ch[50], sat_ch[50], crs_ch[50], alt_ch[50], con_ch[50];  

         
// constructors
WiFiClientSecure wifiConnection;
PubSubClient client(wifiConnection);
TinyGPSPlus gps;
SoftwareSerial gpsSerial(rxPin, txPin);

void brokerConnect(){
  WiFi.begin(ssid, password);        
  
  String client_id = "esp8266-client- " + mac;
  String connectedMac = brokerStatus + mac ;
    
 connectedMac.toCharArray(con_ch, connectedMac.length() + 1);  
  while (WiFi.status() != WL_CONNECTED)
  {
            digitalWrite(LED_BUILTIN, LOW);
            delay(100);
            digitalWrite(LED_BUILTIN, HIGH);
  }
   client.setServer(mqtt_broker, mqtt_port);
     while (!client.connected()) {
    if (client.connect(toCharArray(WiFi.macAddress()), mqtt_uname, mqtt_passwd)) {
    } else {
       digitalWrite(LED_BUILTIN, LOW);
       }
        client.publish(con_ch, "connected" );
 } 
 }


String topics[10] = {"gps/Latitude/", "gps/Longitude/", "gps/spd/", "gps/date/", "gps/time/", "gps/sat/", "gps/crs/", "gps/alt/"};

void prepareTopics(){
for (int i=0; i<=8;i++){
    topics[i] += mac;
}}

void sendGPSData(){
          
    lat_str = String(gps.location.lat(),7); 
    lat_str.toCharArray(lat_ch, lat_str.length()+1); 
    topics[0].toCharArray(latTopic, topics[0].length()+1); 
    client.publish(latTopic,lat_ch); 

     lng_str = String(gps.location.lng(),7); 
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

String startGPS= "";
String callback(char* topic, byte* message, unsigned int length) {
  String messageTemp;
       for (int i = 0; i < length; i++) {
        Serial.print((char)message[i]);
        messageTemp += (char)message[i];
       }
       if (strcmp(topic, locate)==0){
    }
     if (messageTemp == String(WiFi.macAddress())){
            digitalWrite(LED_BUILTIN, LOW);
            delay(100);
            digitalWrite(LED_BUILTIN, HIGH);    
    } 
if (strcmp(topic, gpsstart)==0) {
    if (messageTemp == "1")
    {
      startGPS = "on";
    }
     else if (messageTemp ==  "0")
    {
      startGPS = "off";
    }
   
}

    return startGPS;
}
void setup() {
   pinMode(LED_BUILTIN, OUTPUT);
  gpsSerial.begin(GPSBaud);
  Serial.begin(baudRate);
  wifiConnection.setInsecure();
  brokerConnect();
  client.setCallback(callback);
  client.subscribe(gpsstart);
   client.subscribe(locate);
  prepareTopics();

 }

void loop(){
unsigned long currentMillis = millis();
 client.loop();
  while (gpsSerial.available() > 0 ) {
    if (gps.encode(gpsSerial.read()))
      {
        if (gps.location.isValid() && startGPS == "on")
        {
          
           if (currentMillis - previousMillis >= interval  ) {
                previousMillis = currentMillis;
                sendGPSData();
     
              }
        }
      }
    }
}