/*
TODO:
  Figure out how to allow ValueDisplay to take pointers to other objects
  Implement terminal logging system for terminal using a .log file
  Simplify terminal.println() and sm.update() to one line
  Add a logbook at the end for max speed and such
*/
/*
WIRING:
  OLED Screen:
    GND  -> Ground
    VCC  -> 3.3V
    SCl  -> SCL (21)
    SDA  -> SDA (20)
  GPS:
    GND  -> Ground
    VCC  -> 5.V
    RX   -> TX1 (18)
    TX   -> RX1 (19)
  SD Card:
    GND  -> Ground
    VCC  -> 5V
    MISO -> 51 (Not sure why) (SDI)
    MOSI -> 50 (Not sure why) (SDO)
    CLK  -> 52 (Not sure why)
    CS   -> 8 (Defined in code as CHIP_SELECT)
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
const int8_t OLED_RESET = -1;        // Unused
const int8_t SCREEN_ADDRESS = 0x3C;  /// 3C doesn't work for some reason, even though the screen is 128x65"< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32"
// SD Card
const int8_t CHIP_SELECT = 8;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
TinyGPSPlus gps;

TerminalDisplay terminal(display);
ScreenManager sm(display);
ValueDisplay vd(display, gps);

void setup() {
// Serial
  Serial.begin(9600);
  Serial1.begin(9600);
  while (!Serial || !Serial1);
  Serial.println(Utilities::formatNumber(0, -1, 2));
  Serial.println("\n\nSerial    - Successful");
  terminal.println("Serial    - Successful");

// OLED
  Serial.print("OLED      - ");
  terminal.print("OLED      - ");
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("Failed");
    terminal.println("Failed");
    while(true);
  }
  display.clearDisplay();

  sm.addScreen(&terminal);
  Serial.println("Successful");
  terminal.println("Successful");sm.update();

// SD Card
  terminal.print("SD Card   - ");
  Serial.print("SD Card   - ");
  if (!SD.begin(CHIP_SELECT)) {
    terminal.println("Failed");
    sm.update();
    Serial.println("Failed");
    while(true);
  }
  Serial.println("Successful");
  terminal.println("Successful");sm.update();

// GPS
  Serial.print("GPS - "); 
  terminal.print("GPS - ");sm.update();
  bool timeFlag = false;
  bool positionFlag = false;
  while (true) {
    unsigned long start = millis();
    do {
      while (Serial1.available()) {
        gps.encode(Serial1.read());
      }
    } while (millis()   - start < 1000);
    if (!timeFlag && Utilities::formatDate(gps.date) != "00-00-2000" && Utilities::formatTime(gps.time) != "00:00:00") {
      timeFlag = true;
      Serial.print("DT"); // Synced (D)ate and (T)ime
      terminal.print("DT");sm.update();
    }
    if (!positionFlag && gps.location.isValid() && gps.altitude.isValid()) {
      positionFlag = true;
      Serial.print("P"); // Synced (P)osition
      terminal.print("P");sm.update();
    }
    if (timeFlag && positionFlag) {
      break;
    }
  }
  //previousLocation = gps.location;
  //previousTime = gps.time;
  Serial.println(" - Successful");
  terminal.println(" - Succesful");sm.update();
  //vd.addValue("Time", &ValueDisplay::gettimeofdayseconds);
  //vd.addValue("Lat", &ValueDisplay::getlatitude);
  //vd.addValue("Lng", &ValueDisplay::getlongitude);
  //vd.addValue("MPH", &ValueDisplay::getspeedmph);
  //vd.addValue("Pace", &ValueDisplay::getpaceminpmi);
  vd.addValue("Time (elap)", &ValueDisplay::getelapsedtime);
  vd.addValue("Distance", &ValueDisplay::getdistancemi);
  vd.addValue("Avg. Pace", &ValueDisplay::getaveragepaceminpmi);
  vd.addValue("Avg. Speed", &ValueDisplay::getaveragespeedmph);
  vd.addValue("Speed (rol)", &ValueDisplay::getrollingspeedmph);
  vd.addValue("Elevation", &ValueDisplay::getelevation);
  Serial.print("File      - ");
  terminal.print("File      - ");sm.update();
  vd.am.begin();
  Serial.println(vd.am.getFileName());
  terminal.println(vd.am.getFileName());sm.update();
  sm.addScreen(&vd);
  sm.update();
  delay(2000);
  sm.switchScreen(1);
  sm.update();
  
}
void loop() {
  unsigned long start = millis();
  do {
    while (Serial1.available()) {
      gps.encode(Serial1.read());
    }
  } while (millis() - start < LOOP_INTERVAL);
  vd.am.update();
  vd.am.printGPXtrkpt();
  //terminal.println(Utilities::formatNumber(gps.location.lat(), 2, 7) + ", " + Utilities::formatNumber(gps.location.lng(), 2, 7));
  //terminal.println(Utilities::formatTime(gps.time));
  sm.update();
}
