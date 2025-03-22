#include "Screen.h"
void Screen::setDisplay(Adafruit_SSD1306& disp) {
  display = disp;
}
void Screen::drawTab(uint8_t number, bool open) {
  display.fillRect(number * 28, 0, 27, 15, SSD1306_BLACK);
  if(open) {
    display.fillRoundRect(number * 28, 0, 27, 15, 4, SSD1306_WHITE);
  } else {
    display.drawRoundRect(number * 28, 0, 27, 15, 4, SSD1306_WHITE);
  }
  display.setTextColor(SSD1306_INVERSE);
  display.setCursor(number * 28 + 5, 4);
  display.println(tabName);
}