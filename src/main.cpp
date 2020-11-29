#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <HardwareSerial.h>

// PMS Library, from arduino library manager.
// Mariusz Kacki 1.1.0 
// https://github.com/fu-hsi/pms
#include "PMS.h"

// configure your display type in User_Setup.h of TFT_eSPI library.
TFT_eSPI tft = TFT_eSPI();

// pms sensor setup
HardwareSerial pmsSerial(1);
PMS pms(pmsSerial);
PMS::DATA data;
int pm1 = 0;
int pm25 = 0;
int pm10 = 0;


void setup(void) {
  // display init
  tft.init();
  tft.setRotation(2);
  tft.fillScreen(TFT_BLACK);

  // pms sensor init
  pmsSerial.begin(9600, SERIAL_8N1, 16, 17);
}

void loop() {
  
  if (pms.read(data))
  {
    pm25 = data.PM_AE_UG_2_5;    
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
  tft.print(452);
  tft.println(" ");

  delay(1000);
}