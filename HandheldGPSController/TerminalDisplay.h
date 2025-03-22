#ifndef TERMINALDISPLAY_H
#define TERMINALDISPLAY_H

#include "Screen.h"
#include <Arduino.h>
// Terminal is 6 lines of 21 characters
class TerminalDisplay : public Screen {
private:
  String terminalLines[6]; // Holds the 6 most recent terminal lines, where index 0 is the most recent
  bool onNewLine; // Helps with differentiating between print() and println()
  void shift(); // Shifts all indexes up one (index 0 moves to index 1, etc.)
public:
  TerminalDisplay();
  TerminalDisplay(Adafruit_SSD1306& display);
  void draw() override;
  void print(String line);
  void println(String line);
};

#endif