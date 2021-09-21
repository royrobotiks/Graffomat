/*

Little handheld device to send serial data from SD card to the graffomat.
=========================================================================
Consists of and an KMR1.8 TFT display with integrated SD card reader
Connected with 5-pin XLR to the motor driver unit.

XLR     Arduino   Wire 
Pins    Pins      Colors

 5       +         Red
 4       5(RX)     Green
 2       3(TX)     Blue
 1       -         Black

Programmed with Arduino IDE 1.8.13

* ----------------------------------------------------------------------------
* "THE TSCHUNK-WARE LICENSE" (Revision 23):
* <nikl@s-roy.de> wrote this file. As long as you retain this notice you
* can do whatever you want with this stuff. If we meet some day, and you think
* this stuff is worth it, you can buy me a tschunk in return. Niklas
* ----------------------------------------------------------------------------
*/



#include <SPI.h>
#include <SD.h>
#include <TFT.h>

#define PIN_SD_CS 4
#define PIN_TFT_CS 10
#define PIN_DC 9
#define PIN_RST 8

#define BUTTON_G A4
#define BUTTON_Y A2


#include <SoftwareSerial.h>
SoftwareSerial mySerial(5, 3); // RX, TX

TFT tft = TFT(PIN_TFT_CS, PIN_DC, PIN_RST);

void setup() {
  pinMode(BUTTON_Y, INPUT_PULLUP);
  pinMode(BUTTON_G, INPUT_PULLUP);
  Serial.begin(38400);
  while (!Serial) {
  }
  mySerial.begin(38400); // start software serial in order to communicate with plotter

  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  tft.background(0, 0, 0); // prints black screen to TFT display
  tft.setTextSize(1);
  tft.setCursor(1, 1);
  tft.fill(255, 255, 255);
  tft.stroke(255, 255, 255);
  tft.println("GRAFFOMAT SD-CARD READER");
  tft.println();
  tft.print("OPENING SD-CARD... ");
  if (init_SD()) {
    tft.println("OK!");
    tft.println("");
    tft.println("PRESS ANY BUTTON TO BEGIN.");
     while (digitalRead(BUTTON_Y) && digitalRead(BUTTON_G)); // wait for button press
     tft.background(0, 0, 0);
     delay(100);
     while(!digitalRead(BUTTON_Y) || !digitalRead(BUTTON_G)); // wait for button release
     
  } else {
    tft.println("FAILED!");
    while (1);

  }
}



void loop() {

  File dir = SD.open("/"); // open root path on SD card
  File entry;
  char name[16]; //name of the file
  bool worked_once = false;

  while (entry = dir.openNextFile()) { // iteratively opens all files on SD card.
    Serial.print(F("Opened File: "));
    Serial.println(entry.name());
    strcpy(name, entry.name()); // file name is copied to variable "name"
    entry.close(); // After copying the name, we do not need the entry anymore and, therefore, close it.
    int filename_len = strlen(name);
    if ((filename_len >= 4 && strcmp(name + filename_len - 4, ".PLT") == 0)) { // check if the current filename ends with ".PLT". If so, we might have an image.
      drawFile(name); // plot the image
      while (digitalRead(BUTTON_Y)) { // wait until Y is pushed, then move to next file
        if (!digitalRead(BUTTON_G)) { // if button G is pushed: send file
          sendFile(name);
        }
      }
    }
  }
  dir.close(); // directory is closed
}
