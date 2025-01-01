#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Firebase_ESP_Client.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
LiquidCrystal_I2C I2C_LCD1(0x27, 16, 2);
WiFiUDP udp;
NTPClient timeClient(udp, "pool.ntp.org", 25200, 60000); // Khu vực GMT+7
const char* ssid = "huu nam";
const char* password = "matkhau987" ;

#define API_KEY "AIzaSyCyN4sIv-WSpEOOUcC3g3zsr3oH3Ue896U"
#define DATABASE_URL "https://csdl2024-default-rtdb.asia-southeast1.firebasedatabase.app/"
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
unsigned long sendDataPrevMillis = 0;
bool signupOK = false;
int val = 0;
float Vin;
float power;
const int portPin = 34;
void setup() {
  Serial.begin(115200);
  I2C_LCD1.init();
  I2C_LCD1.backlight();
  WiFi.begin(ssid,password);
  Serial.println("connecting");
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(300);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
 
  if(Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n",config.signer.signupError.message.c_str());
  }
  
  config.token_status_callback = tokenStatusCallback;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  
  timeClient.begin();

}

void loop() {
  delay(1000);
  timeClient.update();
  val = analogRead(portPin);
  Serial.print(val);
  Serial.print(" ");
  Vin = float((val*3.3)/4095.0);
  power = float((Vin*(470000+940000))/470000);
  char P[10];
  String time = String(timeClient.getFormattedTime());

  String uniqueId = time;
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 300 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    Serial.println(uniqueId);
    if(Firebase.RTDB.setFloat(&fbdo,"/VoltageData/"+ uniqueId,power)){
      Serial.print("Send VoltageVoltage data ");
      Serial.println(power);
    }
  }
  sprintf(P,"Power: %.2f V",power);
  
  I2C_LCD1.setCursor(0,0);
  I2C_LCD1.print(P);
  // Serial.print("Power: ");
  // Serial.print(power);
  // Serial.println("V");
  
}
