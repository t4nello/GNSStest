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


// Broker Setup
char *mqtt_broker = "192.168.0.123";
int mqtt_port = 1883;
char *mqtt_uname = "TestUser1";
char *mqtt_passwd = "saper22";

// topics about hardware info
char *gpsstart = "esp/gps/start";
char *brokerStatus = "esp/broker/status";
char *wifissid = "esp/wifi/ssid";
char *gpsstatus= "esp/status/gps";

//GPS data topics 
char *latTopic = "gps/lat";
char *lngTopic = "gps/lng";
char *spdTopic = "gps/spd";
char *dateTopic = "gps/date";
char *timeTopic = "gps/time";
char *satTopic = "gps/sattelites";
char *crsTopic = "gps/course";

// constructors
WiFiClient wifiConnection;
PubSubClient client(wifiConnection);
TinyGPSPlus gps;
SoftwareSerial gpsSerial(rxPin, txPin);


void setup() {
  gpsSerial.begin(GPSBaud);
  Serial.begin(baudRate);
  mqtt_connect();
  client.setCallback(startGPS);
  client.subscribe("esp/incoming");
}


//connect to broker 
void mqtt_connect(){
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED)
  {
   Serial.print("."); // blink led instead of  print 
   delay(500);
  }
   client.setServer(mqtt_broker, mqtt_port);
    if (client.connect("ESP8266", mqtt_uname, mqtt_passwd)) {
         client.publish(brokerStatus, "ESP succesfully connected to Broker");
      } else {
          // add led blinking and reconnect method in future
      }
   
}

  

//callback function to recieve messeages
void startGPS(char* topic, byte* message, unsigned int length) {

  String messageTemp;
    for (int i = 0; i < length; i++) {
        Serial.print((char)message[i]);
        messageTemp += (char)message[i];
    }
}


float latitude, longitude, speed,
          date, _time, satellites, course;

String lat_str, lng_str, spd_str, 
          date_str, time_str, sat_str, crs_str;

char lat_ch[20] , lng_ch[20], spd_ch[20], 
          date_ch[20], time_ch[20], sat_ch[20], crs_ch[20];  


void loop()
{
 client.loop();
 
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read()))
      {
        if (gps.location.isValid())
        {
          Serial.print( gps.course.deg());
          latitude = gps.location.lat();
          lat_str = String(latitude , 6); 
          lat_str.toCharArray(lat_ch, lat_str.length() + 1);
          client.publish(latTopic, lat_ch);
         
          longitude = gps.location.lng();
          lng_str = String(longitude , 6); 
          lng_str.toCharArray(lng_ch, lng_str.length() + 1);      
          client.publish(lngTopic, lng_ch);
         
          speed = gps.speed.kmph();
          spd_str = String(speed); 
          spd_str.toCharArray(spd_ch, spd_str.length() + 1);      
          client.publish(spdTopic, spd_ch);
         
          date = gps.date.value();
          date_str = String(date,0); 
          date_str.toCharArray(date_ch, date_str.length() + 1);      
          client.publish(dateTopic, date_ch);
          
          _time = gps.time.value();
          time_str = String(_time,0); 
          time_str.toCharArray(time_ch, time_str.length() + 1);      
          client.publish(timeTopic, time_ch);
         
          satellites =  gps.satellites.value();
          sat_str = String(satellites); 
          sat_str.toCharArray(sat_ch, sat_str.length() + 1); 
          client.publish(satTopic, sat_ch);
         
          course = gps.course.deg();
          crs_str = String(course,0); 
          crs_str.toCharArray(crs_ch, crs_str.length() + 1); 
          client.publish(crsTopic, crs_ch);
               delay (3000);
          }
       }
    }
}