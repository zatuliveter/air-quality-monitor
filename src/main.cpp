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
int pm25Prev = 0;

// mhz sensor setup
HardwareSerial mhzSerial(2);
MHZ19 mhz; 
int co2 = 0;
int co2Prev = -1;


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

void displayData()
{  
  tft.setTextDatum(TL_DATUM);
  tft.setTextColor(TFT_WHITE, TFT_BLACK); 
  tft.setTextFont(4);
  tft.setTextSize(1);
  tft.drawString("PM 2.5:", 0, 0);
  
  if (pm25 != pm25Prev)
  {
    uint pm25Color;
    if (pm25 <= 15)       pm25Color = TFT_GREEN;
    else if (pm25 <= 25)  pm25Color = TFT_GREENYELLOW;
    else if (pm25 <= 75)  pm25Color = TFT_ORANGE;
    else if (pm25 <= 150) pm25Color = TFT_RED;
    else                  pm25Color = TFT_VIOLET;

    tft.setTextFont(7);
    tft.setTextSize(1);    
    tft.setTextDatum(TC_DATUM);
    
    tft.setTextColor(TFT_BLACK, TFT_BLACK);  
    tft.drawNumber(pm25Prev, 64, 25);

    tft.setTextColor(pm25Color, TFT_BLACK);          
    tft.drawNumber(pm25, 64, 25);

    pm25Prev = pm25;
  }

  tft.setTextDatum(TL_DATUM);
  tft.setTextColor(TFT_WHITE, TFT_BLACK); 
  tft.setTextFont(4);
  tft.setTextSize(1);
  tft.drawString("CO2:", 0, 77);
   
  if (co2 != co2Prev)
  {
    uint co2Color;
    if (co2 <= 500)       co2Color = TFT_GREEN;
    else if (co2 <= 600)  co2Color = TFT_GREENYELLOW;
    else if (co2 <= 800)  co2Color = TFT_ORANGE;
    else if (co2 <= 1000) co2Color = TFT_RED;
    else                  co2Color = TFT_VIOLET;
        
    tft.setTextFont(7);
    tft.setTextSize(1);    
    tft.setTextDatum(TC_DATUM);

    tft.setTextColor(TFT_BLACK, TFT_BLACK);  
    tft.drawNumber(co2Prev, 64, 102);
        
    tft.setTextColor(co2Color, TFT_BLACK); 
    tft.drawNumber(co2, 64, 102);
    
    co2Prev = co2;
  }
}


unsigned long getDataTimer = 0;
void loop() {
  
  if (pms.read(data))
  {
    pm25 = data.PM_AE_UG_2_5;    
  }  

  if (millis() - getDataTimer >= 1000)
  {
    co2 = mhz.getCO2();
    
    displayData();

    getDataTimer = millis();
  }
}
