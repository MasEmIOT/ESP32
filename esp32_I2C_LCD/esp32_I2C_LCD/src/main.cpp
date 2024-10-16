#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C I2C_LCD1(0x27, 16, 2);

void setup(){
  I2C_LCD1.init();
  I2C_LCD1.backlight();
  I2C_LCD1.print("Hello World!");
  I2C_LCD1.setCursor(0,1);
  I2C_LCD1.print("Done!");
}

void loop(){
  for(int i=0; i < 7; i++){
    I2C_LCD1.scrollDisplayRight();
    delay(500);
  }

  for(int i = 0; i < 7; i++){
    I2C_LCD1.scrollDisplayLeft();
    delay(500);
  }
}