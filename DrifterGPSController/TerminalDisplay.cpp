#include "Terminaldisplay.h"


TerminalDisplay::TerminalDisplay() : Screen("TRM") {
  onNewLine = true;
  for(int8_t i = 0; i < 6; i++) {
    terminalLines[i] = "";
  }
}
TerminalDisplay::TerminalDisplay(Adafruit_SSD1306& display) : Screen(display, "TRM") {
  onNewLine = true;
  for(int8_t i = 0; i < 6; i++) {
    terminalLines[i] = "";
  }
}
void TerminalDisplay::draw() {
  display.fillRect(0, 16, 128, 48, SSD1306_BLACK);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setTextWrap(false);
  for(int8_t i = 0; i < 6; i++) {
    display.setCursor(0, 16 + 8 * (i));
    display.print(terminalLines[5-i]);
  }
}
void TerminalDisplay::print(String line) {
  if(onNewLine) {
    onNewLine = false;
    shift();
  }
  terminalLines[0] += line;
}
void TerminalDisplay::println(String line) {
  if(onNewLine) {
    onNewLine = false;
    shift();
  }
  terminalLines[0] += line;
  onNewLine = true;
}
void TerminalDisplay::shift() {
  for(int8_t i = 5; i > 0; i--) {
    terminalLines[i] = terminalLines[i - 1];
  }
  terminalLines[0] = "";
}