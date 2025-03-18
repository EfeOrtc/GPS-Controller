/*
TODO:
  Figure out how to allow ValueDisplay to take pointers to other objects
  Implement terminmal logging system for terminal using a .log file
*/
#include <TinyGPSPlus.h>
#include <Arduino.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Screen.h"
#include "TerminalDisplay.h"
#include "ValueDisplay.h"
#include "ScreenManager.h"
// Constants
const int LOOP_INTERVAL = 1000;
// OLED Screen
const uint8_t SCREEN_WIDTH = 128;
const uint8_t SCREEN_HEIGHT = 64;
const int8_t OLED_RESET = -1;  // Unused
const int8_t SCREEN_ADDRESS = 0x3C; /// 3C doesn't work for some reason, even though the screen is 128x65"< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32"
// SD Card
const int8_t CHIP_SELECT = 8;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
TinyGPSPlus gps;

TerminalDisplay terminal(display);
ScreenManager sm(display);
ValueDisplay vd(display, gps);

void setup() {
  pinMode(47, OUTPUT); // For GPS power
  digitalWrite(47, HIGH);
  pinMode(45, OUTPUT); // For OLED power
  digitalWrite(45, HIGH);
  pinMode(LED_BUILTIN, OUTPUT);// For LED
  Serial.begin(9600);
  delay(1000);
  Serial.println("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    //Serial.println("Failed OLED allocation");
    Serial.println("BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB");
    delay(1000);
    while(true);
  }
  display.clearDisplay();
  display.display();
  
  sm.addScreen(&terminal);
  //vd.addValue("Time", &ValueDisplay::gettimeofdayseconds); // Is it really a problem?
  //sm.addScreen(&vd);
}
void loop() {
  display.clearDisplay();
  terminal.println(String(millis() / 1000));
  sm.draw();
  display.display();
  delay(1000);
}
// Output variables
/*
void setup() {
  // LEDS
  pinMode(47, OUTPUT); // For GPS power
  digitalWrite(47, HIGH);
  pinMode(45, OUTPUT); // For OLED power
  digitalWrite(45, HIGH);
  pinMode(LED_BUILTIN, OUTPUT);// For LED

// Setting up the Serial
  Serial.begin(9600);   // The Serial port of Arduino baud rate.
  delay(1000);
  Serial.println("\n\n");
// Setting up the OLED
  Serial.print("Step 1");
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("Failed OLED allocation");
    while(true);
  }
  Serial.println("Step 2");
}

void loop () {
  display.clearDisplay();
  String val = String(millis() / 1000);
  terminal.println(val);
  Serial.println(val);
  terminal.draw();
  terminal.drawTab(1, true);
  
  display.display();
  delay(1000);
}
*/

