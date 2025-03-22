#ifndef SCREEN_H
#define SCREEN_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
class Screen {
protected:
  Adafruit_SSD1306& display;
public:
  String tabName;
  Screen (Adafruit_SSD1306& display, String tabName) : display(display), tabName(tabName) {}
  Screen (String tabName) : tabName(tabName) {}
  void setDisplay(Adafruit_SSD1306& disp);
  void drawTab(uint8_t number, bool open); // Draws the tab on the top with dimensions 27 wide 15 tall
  virtual void draw() = 0; // Draws the screen beneath the tabs with dimensions of 128 wide 64 tall drawn from (0, 16)
};

#endif 