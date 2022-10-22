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

         
// constructors
WiFiClientSecure wifiConnection;
PubSubClient client(wifiConnection);
TinyGPSPlus gps;
SoftwareSerial gpsSerial(rxPin, txPin);

  
   
char* toCharArray(String str) {
  return &str[0];
}

String datalog[]= {  String(gps.location.lat(),6), String(gps.location.lng(),6)};
//,  gps.location.lng(),gps.speed.kmph(), gps.date.value(), gps.time.value(),gps.altitude.meters(), gps.satellites.value()};
auto dataSize = std::size(datalog);
char *prepareTopic (String input){
char ready[50]; 
  String mac =toCharArray(WiFi.macAddress());
  String output= input+mac; 
  output.toCharArray(ready, output.length() + 1);  
  return ready;
}

char* prepareData (double input){
         String stringified = String(input , 6); 
         char ready[50]; 
        stringified.toCharArray(ready, stringified.length() + 1);  
       return ready;
}

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
         Serial.print (ready);
          Serial.print("piwo");

 } 
 }
String passValue = "";

String startGPS(char* topic, byte* message, unsigned int length) {

 passValue= "";
    for (int i = 0; i < length; i++) {
        Serial.print((char)message[i]);
        passValue += (char)message[i];
    }
    return passValue;
}

char* tocharArray(String str) {
  return &str[0];
}

void sendGpsData(){
       for (int i = 1; i<=dataSize; i++){
          
          Serial.println(datalog[i]);  
       }
          }



void setup() {
  gpsSerial.begin(GPSBaud);
  Serial.begin(baudRate);
  wifiConnection.setInsecure();
  brokerConnect();
  client.setCallback(startGPS);
  client.subscribe("esp/incoming");
 
}

void loop()
{

unsigned long currentMillis = millis();
//sendGpsData();
 client.loop();
  while (gpsSerial.available() > 0 ) {
    if (gps.encode(gpsSerial.read()))
      {
        if (gps.location.isValid() && passValue  == "on" )
        {
          
           if (currentMillis - previousMillis >= interval  ) {
                previousMillis = currentMillis;
                sendGpsData();
     
              }
        }
      }
    }
}