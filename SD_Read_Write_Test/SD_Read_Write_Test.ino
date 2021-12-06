#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <Adafruit_GFX.h>         // Core graphics library
#include "Adafruit_ThinkInk.h"
#include <Adafruit_ImageReader_EPD.h>
#include <SdFat.h>

#define SD_CS       5
#define SRAM_CS     32
#define EPD_CS      15
#define EPD_DC      33  
#define EPD_RESET   -1 // can set to -1 and share with microcontroller Reset!
#define EPD_BUSY    -1 // can set to -1 to not use a pin (will wait a fixed delay)
ThinkInk_290_Tricolor_Z10 display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY);

SdFat SD;
File file;
#define FILENAME1 "values.txt"

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
Serial.println();
if(!SD.begin(14, SD_SCK_MHZ(10))) {
  Serial.println(F("SD begin() Failed"));
  for(;;);
}
Serial.println("Setup");
Serial.println();
/*
file=SD.open(FILENAME1,FILE_WRITE);
if(file){
  Serial.println(FILENAME1);
  Serial.println("Writing...");
  file.println("");
  file.println("1");
  file.println("2");
  Serial.println("Finished");
  file.close();
  }
 else {
  Serial.println("error");
}
*/
writeSD(FILENAME1,"12");
readSD(FILENAME1);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void writeSD(char *filename,String val){
  file=SD.open(filename,FILE_WRITE);
  if(file){
    Serial.println(filename);
    Serial.println("Writing...");
    file.println(val);
    Serial.println("Finished");
    file.close();
    }
  else {
    Serial.println("error with write");
    }
  
}

void readSD(char *filename){
  file = SD.open(filename);
  if(file){
    Serial.println("READ");
    Serial.println(filename);
    while(file.available()){
      byte a = file.read();
      Serial.write(a);
    }
    file.close();
  }
  else{
    Serial.println("error with read");
  }
}
