#include <Arduino.h>
#include <WiFi.h>
const char *ssid = "huu nam";
const char *password = "matkhau987";

void setup(){
  Serial.begin(115200);
  Serial.println();
  Serial.println("connect to AP...");
  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.localIP());
}

void loop(){
  
}