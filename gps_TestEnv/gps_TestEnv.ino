#include <ESP8266WiFi.h>
#include <PubSubClient.h>

\\Wi-Fi information
#define ssid "ESPtest"
#define password "MQTTtest2137!"

\\contstructor
WiFiClient wifiConnection;
PubSubClient client(wifiConnection;);

\\connect to wi-fi method

void wifi_connect(){ 
  WiFi.begin(ssid, password);             
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500);
  }
  Serial.println("Connected to network with ip:");  
  Serial.println(WiFi.localIP());        
}


void setup() {
  Serial.begin(115200);
  wifi_connect();

}

void loop() {
 

}
