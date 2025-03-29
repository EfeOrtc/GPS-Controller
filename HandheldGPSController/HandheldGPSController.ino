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
    MISO -> 50 (Not sure why) (SDI)
    MOSI -> 51 (Not sure why) (SDO)
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

// Hardware
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
TinyGPSPlus gps;

// Screens
ScreenManager sm(display);
TerminalDisplay terminal(display);
ValueDisplay vd(display, gps);

// Variables
String folderName;
String fileNameGPX;

void setup() {
// Serial
  Serial.begin(9600);
  Serial1.begin(9600);
  while (!Serial || !Serial1);
  Serial.println("\n\nSerial    - Done");
  terminal.println("Serial    - Done");

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
  Serial.println("Done");
  terminal.println("Done");sm.update();

// SD Card
  Serial.print("SD Card   - ");
  terminal.print("SD Card   - ");sm.update();
  if (!SD.begin(CHIP_SELECT)) {
    terminal.println("Failed");
    sm.update();
    Serial.println("Failed");
    while(true);
  }
  Serial.println("Done");
  terminal.println("Done");sm.update();

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
  Serial.println(" - Done");
  terminal.println(" - Done");sm.update();

// Display
  //vd.addValue("Time of day", &ValueDisplay::gettimeofdayseconds);
  vd.addValue("Time Passed", &ValueDisplay::getelapsedtime);
  vd.addValue("   Distance", &ValueDisplay::getdistancemi);
  vd.addValue("  Avg. Pace", &ValueDisplay::getaveragepaceminpmi);
  vd.addValue("Roll.  Pace", &ValueDisplay::getrollingpaceminpmi);
  //vd.addValue("  Elevation", &ValueDisplay::getelevation);

// File
  Serial.print("File - ");
  terminal.print("File - ");sm.update();
  folderName = Utilities::formatNumber(gps.date.month(), 2, 0) + Utilities::formatNumber(gps.date.day(), 2, 0) + Utilities::formatNumber(gps.time.hour(), 2, 0) + Utilities::formatNumber(gps.time.minute(), 2, 0);
  if (SD.mkdir(folderName)) {
    Serial.print("F");
    terminal.print("F");sm.update();
  } else {
    Serial.println("Failed");
    terminal.print("Failed");sm.update();
  }
  fileNameGPX = folderName + "/route.gpx";
  SD.open(fileNameGPX, FILE_WRITE).close();
  //SD.open(folderName + "/terminal.log", FILE_WRITE).close();
  SD.open(folderName + "/distance.txt", FILE_WRITE).close();
  Serial.print("F - ");
  terminal.print("F - ");sm.update();
  Serial.println(folderName);
  terminal.println(folderName);sm.update();
  sm.addScreen(&vd);
  sm.update();
// Syncing GPS so the start isn't weird
  Serial.println("Allow 30s for GPS");
  terminal.println("Allow 30s for GPS");
  Serial.print("30");
  terminal.print("30");sm.update();
  for(int8_t i = 0; i < 6; i++) {
    // Updating GPS
    unsigned long start = millis();
    do {
    while (Serial1.available()) {
      gps.encode(Serial1.read());
    }
    } while (millis() - start < 5000);
    // Outputting
    Serial.print("-" + String((25 - (i * 5))));
    terminal.print("-" + String((25 - (i * 5))));sm.update();
  }
  vd.am.begin(fileNameGPX);
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
  Utilities::printlnToFile(vd.getdistancemi(), folderName + "/distance.txt");
  sm.update();
}
