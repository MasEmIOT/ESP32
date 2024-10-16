#include <Arduino.h>
#include <WiFi.h>
const char *ssid = "NTD_MasIOT";
const char *password = "05022004";

void setup(){
  Serial.begin(115200);
  WiFi.softAP(ssid,password);
}

void loop(){
  
}