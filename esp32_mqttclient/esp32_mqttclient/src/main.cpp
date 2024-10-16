#include <Arduino.h>
#include<Wire.h>
#include <WiFi.h>
#include <string.h>
#include <PubSubClient.h>
const char* ssid = "huu nam";
const char* password = "matkhau987" ;

//const char* mqtt_server = "192.168.1.180";
const char* mqtt_server = "192.168.142.223";
const char* device_name = "ESP32";
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

WiFiClient espClient;
PubSubClient client(espClient);
long now = millis();
long lastMeasure = 0;

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

void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    if (client.connect("ESP32Client")) {
      Serial.println("connected");  
      client.subscribe("esp32/4");
      client.subscribe("esp32/5");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(15000);
    }
  }
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
  client.setServer(mqtt_server,1883);
  client.setCallback(callback);
}
 
void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
  }
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,6,true);
  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop())

    client.connect("ESP32Client");
    
  now = millis();
  if (now - lastMeasure > 10000) {
    lastMeasure = now;

  AccX =(float)(Wire.read() << 8 |Wire.read())/16384.0;
  AccY =(float)(Wire.read() << 8 |Wire.read())/16384.0;
  AccZ =(float)(Wire.read() << 8 |Wire.read())/16384.0;
}
  // Wait a few seconds between measurements.
  
  // AccX =(float)(Wire.read() << 8 |Wire.read())/16384.0;
  // AccY =(float)(Wire.read() << 8 |Wire.read())/16384.0;
  // AccZ =(float)(Wire.read() << 8 |Wire.read())/16384.0;
  char accX[10], accY[10], accZ[10];
  sprintf(accX,"%f",AccX);
  sprintf(accY,"%f",AccY);
  sprintf(accZ,"%f",AccZ);
  Serial.print(accX);
  Serial.print(",");
  Serial.print(accY);
  Serial.print(",");
  Serial.println(accZ);
  
  client.publish("/esp32/AccX",accX);
  client.publish("/esp32/AccY",accY);
  client.publish("/esp32/AccZ",accZ);
  }
  
