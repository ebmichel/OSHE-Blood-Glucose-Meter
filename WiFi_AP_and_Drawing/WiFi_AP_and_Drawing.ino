/*
  WiFiAccessPoint.ino creates a WiFi access point and provides a web server on it.

  Steps:
  1. Connect to the access point "yourAp"
  2. Point your web browser to http://192.168.4.1/H to turn the LED on or http://192.168.4.1/L to turn it off
     OR
     Run raw TCP "GET /H" and "GET /L" on PuTTY terminal with 192.168.4.1 as IP address and 80 as port

  Created for arduino-esp32 on 04 July, 2018
  by Elochukwu Ifediora (fedy0)
*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <Adafruit_GFX.h>         // Core graphics library
#include "Adafruit_ThinkInk.h"
#include <Adafruit_ImageReader_EPD.h> // Image-reading functions


#define SD_CS       5
#define SRAM_CS     32
#define EPD_CS      15
#define EPD_DC      33  
#define EPD_RESET   -1 // can set to -1 and share with microcontroller Reset!
#define EPD_BUSY    -1 // can set to -1 to not use a pin (will wait a fixed delay)
ThinkInk_290_Tricolor_Z10 display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY);


#define BUTTON1 27
#define BUTTON2 12
#define BUTTON3 13
#define COLOR1 EPD_BLACK
#define COLOR2 EPD_RED


//#define LED_BUILTIN 2   // Set the GPIO pin where you connected your test LED or comment this line out if your dev board has a built-in LED

// Set these to your desired credentials.
const char *ssid = "yourAP";
const char *password = "yourPassword";

WiFiServer server(80);


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  Serial.println();

  display.begin();
  display.clearBuffer();
  display.display();
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");
  pinMode(BUTTON1, INPUT_PULLUP); 
  pinMode(BUTTON2, INPUT_PULLUP); 
  pinMode(BUTTON3, INPUT_PULLUP);
}

void loop() {
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            
            // the content of the HTTP response follows the header:
            //client.print("Click <a href=\"/H\">here</a> to turn ON the LED.<br>");
            //client.print("Click <a href=\"/L\">here</a> to turn OFF the LED.<br>");
            client.print("Click <a href=\"/A\">here</a> <br>");
            client.print("test");
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
        if (currentLine.endsWith("GET /A")) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();
          display.clearBuffer();
          int data[6]={1,2,3,4,5,6};
          display.setCursor(0, 0);
          display.setTextColor(COLOR1);
          display.setTextWrap(true);
          display.println("Previous six measurements:");
          client.print("Previous six measurements:");
          for (byte i =0; i<6; i=i+1){
          display.println(String(data[i]));
          client.print(String(data[i]));
          }
          display.display();
        }
            // GET /L turns the LED off
        }
      }
    
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}

void testdrawtext(const char *text, uint16_t color) {
  display.setCursor(0, 0);
  display.setTextColor(color);
  display.setTextWrap(true);
  display.print(text);
}
