#include <Arduino.h>
#include<Wire.h>
#include <WiFi.h>
#include <string.h>
#include <HTTPClient.h>
const char* ssid = "huu nam";
const char* password = "matkhau987" ;
const char* host = "192.168.1.180";
//String url = "http://192.168.1.180/index.php?";
#define VBR 5

#define I2C_SDA 16
#define I2C_SCL 17

#define ACCEL_XOUTH 0x3B
#define ACCEL_XOUTL 0x3C
#define ACCEL_YOUTH 0x3D
#define ACCEL_YOUTL 0x3E
#define ACCEL_ZOUTH 0x3F
#define ACCEL_ZOUTL 0x40
#define GYRO_XH 0x43
#define GYRO_XL 0x44
const int MPU = 0x68;
float AccX,AccY,AccZ;

long vibration()
{
  long measurement = pulseIn(VBR,HIGH);
  return measurement;
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}


void setup() {
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();
  pinMode(VBR, INPUT);
  Serial.begin(115200);
  
    //setup wifi
  setup_wifi();
}
 
void loop() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,6,true);
  // Wait a few seconds between measurements.
  
  AccX =(float)(Wire.read() << 8 |Wire.read())/16384.0;
  AccY =(float)(Wire.read() << 8 |Wire.read())/16384.0;
  AccZ =(float)(Wire.read() << 8 |Wire.read())/16384.0;
  long SW420 = vibration();
  char accX[10], accY[10], accZ[10];
  sprintf(accX,"%f",AccX);
  sprintf(accY,"%f",AccY);
  sprintf(accZ,"%f",AccZ);
  Serial.println(SW420);
  Serial.print(accX);
  Serial.print(",");
  Serial.print(accY);
  Serial.print(",");
  Serial.println(accZ);
  WiFiClient client;
  const int httpPort = 80;
  if(!client.connect(host,httpPort)){
    Serial.println("connection failed");
    return;
  }
  
  client.print(String("POST http://192.168.1.180/index.php?") + ("&SW420=") + SW420 + ("&AccX=") + AccX + ("&AccY=") + AccY + ("&AccZ=") + AccZ + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
  // HTTPClient http;
  // http.begin(url);
  // String postData = "SW420=" + String(SW420) + "&AccX=" + String(AccX) + "&AccY" + String(AccY) + "&AccZ" + String(AccZ);
  // int httpCode = http.POST(postData);
  // String payload = http.getString();

  unsigned long timeout = millis();
  while(client.available() == 0){
    if(millis() - timeout > 1000){
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
    while(client.available()) {
        String line = client.readStringUntil('\r');
        Serial.print(line);
        
    }

    Serial.println();
    Serial.println("closing connection");
}
  
