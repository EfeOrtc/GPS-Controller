#ifndef VALUEDISPLAY_H
#define VALUEDISPLAY_H

#include "Screen.h"
#include <Arduino.h>
#include "ActivityMonitor.h"
class ValueDisplay : public Screen {
private:
  String titles[6]; // Holds the titles, 0th index is drawn at the top
  String (ValueDisplay::*values[6])(); // Holds the function pointers
  uint8_t numValues; // Current number of values
public:
  ActivityMonitor am;
  ValueDisplay(Adafruit_SSD1306& display, TinyGPSPlus& gps) : Screen(display, "DIS"), am(gps), numValues(0), titles({""}), values({nullptr}) {}
  void draw() override;
  void addValue(String title, String (ValueDisplay::*value)()); // Inserts the title and value at the end
  void addValue(String title, String (ValueDisplay::*value)(), uint8_t index); // Inserts the title and value at the given index
  void removeValue(uint8_t index);
  // Value functions
  String getlatitude();
  String getlongitude();
  String getaveragespeedkmph();
  String getaveragespeedmph();
  String getspeedkmph();
  String getspeedmph();
  String getmaxspeedkmph();
  String getmaxspeedmph();
  String getrollingspeedkmph();
  String getrollingspeedmph();
  String getaveragepaceminpkm();
  String getaveragepaceminpmi();
  String getpaceminpkm();
  String getpaceminpmi();
  String getrollingpaceminpkm();
  String getrollingpaceminpmi();
  String getdistancekm();
  String getdistancemi();
  String getheading();
  String getelevation();
  String getelapsedtime();
  String getdate();
  String gettimeofday();
  String gettimeofdayseconds();
};

#endif

/* Options
latitude
longitude
averagespeedkmph
averagespeedmph
speedkmph
speedmph
maxspeedkmph
maxspeedmph
rollingspeedkmph
rollingspeedmph
averagepaceminpkm
averagepaceminpmi
paceminpkm
paceminpmi
rollingpaceminpkm
rollingpaceminpmi
distancekm
distancemi
heading
elevation
elapsedtime
date
timeofday
timeofdayseconds
*/
