#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <HardwareSerial.h>

// PMS Library, from arduino library manager.
// Mariusz Kacki 1.1.0 
// https://github.com/fu-hsi/pms
#include "PMS.h"

// MH-Z19 Arduino library, v.1.5.2
// Jonathan Dempsey
// https://github.com/WifWaf/MH-Z19
#include "MHZ19.h"

// configure your display type in User_Setup.h of TFT_eSPI library.
TFT_eSPI tft = TFT_eSPI();

// pms sensor setup
HardwareSerial pmsSerial(1);
PMS pms(pmsSerial);
PMS::DATA data;
int pm25 = 0;

// mhz sensor setup
HardwareSerial mhzSerial(2);
MHZ19 mhz; 
int co2 = 0;


void setup(void) {
  // display init
  tft.init();
  tft.setRotation(2);
  tft.fillScreen(TFT_BLACK);

  // pms sensor init
  pmsSerial.begin(9600, SERIAL_8N1, 16, 17);
  
  // MH-Z19 CO2 Sensor Config:
  mhzSerial.begin(9600, SERIAL_8N1, 4, 0);
  mhz.begin(mhzSerial); 
  mhz.autoCalibration();   
}

unsigned long getDataTimer = 0;

void loop() {
  
  if (pms.read(data))
  {
    pm25 = data.PM_AE_UG_2_5;    
  }  

  if (millis() - getDataTimer >= 2500)
  {
    co2 = mhz.getCO2();
    
    getDataTimer = millis();
  }

  // Set "cursor" at top left corner of display (0, 0) and select font 2
  // (cursor will move to next line automatically during printing with 'tft.println'
  //  or stay on the line is there is room for the text with tft.print)
  tft.setCursor(0, 0, 4);
  tft.setTextColor(TFT_WHITE, TFT_BLACK); 
  tft.setTextSize(1);
  // We can now plot text on screen using the "print" class
  tft.println("pm2.5:");
  
  tft.setTextColor(TFT_ORANGE, TFT_BLACK); 
  tft.setTextFont(7);
  tft.setTextSize(1);
  tft.print(pm25);
  tft.println(" ");
  
  tft.setTextFont(4);
  tft.setTextColor(TFT_WHITE, TFT_BLACK); 
  tft.setTextSize(1);
  tft.println("CO2:");
 
  tft.setTextColor(TFT_GREEN, TFT_BLACK); 
  tft.setTextFont(7);
  tft.setTextSize(1);
  tft.print(co2);
  tft.println(" ");

  delay(1000);
}