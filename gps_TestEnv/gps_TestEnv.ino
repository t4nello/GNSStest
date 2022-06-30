#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Wi-Fi information
#define ssid "ESPtest"
#define password "MQTTtest2137!"

// MQTT setup
char *mqtt_broker = "broker.emqx.io";
int mqtt_port = 1883;
char *mqtt_uname= "test";
char *mqtt_passwd= "test";
char *sendTopic= "esp/sendtopic";
char *reciveTopic= "esp/recivetopic";

// constructors
WiFiClient wifiConnection;
PubSubClient client(wifiConnection);


// connect to wi-fi method
void wifi_connect(){ 
  WiFi.begin(ssid, password);             
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500);
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
  Serial.print("Message arrived in topic: ");
  Serial.println(reciveTopic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
      Serial.print((char) payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");
}


void setup() {

  Serial.begin(115200);
  client.setCallback(callback);
  wifi_connect();
  mqtt_connect();
  
}

void loop() {
 client.loop();

}
