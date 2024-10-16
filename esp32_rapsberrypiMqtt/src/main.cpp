#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
const char* ssid = "huu nam";
const char* password = "matkhau987" ;

//const char* mqtt_server = "192.168.1.180";
const char* mqtt_server = "192.168.142.223";

WiFiClient espClient;
PubSubClient client(espClient);
char msg[50];
int value = 0;
const char* device_name = "ESP32";

// void setup_wifi() {
//   Serial.print(" ");
//   Serial.print("Connecting to Wifi...");
//   WiFi.begin(ssid,password);
//   while(WiFi.status() != WL_CONNECTED){
//     delay(500);
//     Serial.print(".");
//   }
//   Serial.println("");
//   Serial.println("Wifi connected.");
//   Serial.print("IP address: ");
//   Serial.print(WiFi.localIP());
  
// }

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  if(String(topic) == "My_Topic"){
    Serial.print("Changing output to ");
    if(messageTemp == "message"){
      client.publish("My_Topic","Message Recieved at ESP32");
      Serial.println("on");
    }
    else if(messageTemp == "off"){
      Serial.println("off");
    }
  }
}

void reconnect() {
  while (!client.connected()) {  
        Serial.print("Attempting MQTT connection...");
        if (client.connect(device_name)) { 
          Serial.println("connected");
          client.subscribe("My_Topic");
        } 
        else {
          Serial.print("failed, rc=");
          Serial.print(client.state());
          Serial.println(" trying again in 2 seconds");
          delay(2000);
        }
  }
  
}

void setup(){
  Serial.begin(115200);
  //setup_wifi();
  Serial.print(" ");
  Serial.print("Connecting to Wifi...");
  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Wifi connected.");
  Serial.print("IP address: ");
  Serial.print(WiFi.localIP());
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop(){
  if(!client.connected()){
     reconnect();
   }
   client.loop();
}