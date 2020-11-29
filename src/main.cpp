#include <Arduino.h>


/*  
 Test the tft.print() viz the libraries embedded write() function

 This sketch used font 2, 4, 7
 
 Make sure all the required fonts are loaded by editting the
 User_Setup.h file in the TFT_eSPI library folder.

  #########################################################################
  ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
  #########################################################################
 */

#include <SPI.h>

#include <TFT_eSPI.h> // Hardware-specific library


TFT_eSPI tft = TFT_eSPI();


void setup(void) {
  tft.init();
  tft.setRotation(2);

  // Fill screen with random colour so we can see the effect of printing with and without 
  // a background colour defined
  tft.fillScreen(TFT_BLACK);
}

void loop() {
    
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
  tft.print(57);
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