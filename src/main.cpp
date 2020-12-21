#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <HardwareSerial.h>

// BME related header files
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

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
int pm25Prev = -1;

// mhz sensor setup
HardwareSerial mhzSerial(2);
MHZ19 mhz; 
int co2 = 0;
int co2Prev = -1;

// bme sensor setup
Adafruit_BME280 bme; // I2C default pins (SCL=IO22, SDA=IO21 on ESP32)
float temp = 0;
float pressure = 0;
float humidity = 0;


uint16_t labelColor = tft.color565(230, 220, 160);

void setup(void) {
  // display init
  tft.init();
  tft.setRotation(4);
  tft.fillScreen(TFT_BLACK);

  // pms sensor init
  pmsSerial.begin(9600, SERIAL_8N1, 16, 17);
  
  // MH-Z19 CO2 Sensor Config:
  mhzSerial.begin(9600, SERIAL_8N1, 4, 0);
  mhz.begin(mhzSerial);
  mhz.setRange(2000);
  mhz.setFilter();
  mhz.autoCalibration(); 
  
  // BME280 Sensor Config:
  if (! bme.begin(0x76, &Wire)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

}

void displayData()
{ 
  if (pm25 != pm25Prev)
  {
    uint pm25Color = TFT_VIOLET;
    if (pm25 <= 150) pm25Color = TFT_RED;
    if (pm25 <= 75)  pm25Color = TFT_ORANGE;
    if (pm25 <= 25)  pm25Color = TFT_GREENYELLOW;
    if (pm25 <= 15)  pm25Color = TFT_GREEN;
    
    tft.setTextFont(7);
    tft.setTextSize(1);    
    tft.setTextDatum(TC_DATUM);
    
    tft.setTextColor(TFT_BLACK);  
    tft.drawNumber(pm25Prev, 64, 25);

    tft.setTextColor(pm25Color);          
    tft.drawNumber(pm25, 64, 25);

    pm25Prev = pm25;
  }

  if (co2 != co2Prev)
  {
    uint co2Color = TFT_VIOLET;
    if (co2 <= 1000) co2Color = TFT_RED;                    
    if (co2 <= 800)  co2Color = TFT_ORANGE;
    if (co2 <= 600)  co2Color = TFT_GREENYELLOW;
    if (co2 <= 500)  co2Color = TFT_GREEN;
       
    tft.setTextFont(7);
    tft.setTextSize(1);    
    tft.setTextDatum(TC_DATUM);
    
    tft.setTextColor(TFT_BLACK);  
    if (co2Prev == 0) {
      tft.drawString("---", 64, 80);
    }
    else {
      tft.drawNumber(co2Prev, 64, 80);
    }
        
    if (co2 == 0) {
      tft.setTextColor(labelColor); 
      tft.drawString("---", 64, 80);
    }
    else {
      tft.setTextColor(co2Color); 
      tft.drawNumber(co2, 64, 80);
    }
    
    co2Prev = co2;
  }
  
  tft.setTextDatum(TL_DATUM);
  tft.setTextColor(labelColor); 
  tft.setTextFont(2);
  tft.setTextSize(1);
  int y = 0;
  tft.drawString("PM", 0, y + 32);
  tft.drawString("2.5", 0, y + 48);
  
  tft.drawString("C", 0, y + 80);
  tft.drawString("O", 0, y + 93);  
  tft.drawString("2", 0, y + 106);
  

  tft.setTextDatum(TL_DATUM);
  tft.setTextColor(labelColor, TFT_BLACK); 
  tft.setTextFont(4);
  tft.setTextSize(1);
  
  String strTempAndHumidity = String(temp, 1) + "`  " + String(humidity, 0) + "%";
  tft.setTextDatum(TC_DATUM);
  tft.drawString(strTempAndHumidity, 64, 0);
  
  String strPressure = String(pressure, 2) + " kPa";
  tft.setTextDatum(TC_DATUM);
  tft.drawString(strPressure, 64, 133);
  
}


unsigned long getDataTimer = 0;

void loop() 
{  
  if (pms.read(data))
  {
    pm25 = data.PM_AE_UG_2_5;    
  }  

  if (millis() - getDataTimer >= 1000)
  {
    // reading co2
    co2 = mhz.getCO2();

    // reading bme
    temp = bme.readTemperature();
    pressure = bme.readPressure() / 1000.0F;
    humidity = bme.readHumidity();
    
    displayData();

    getDataTimer = millis();
  }
}
