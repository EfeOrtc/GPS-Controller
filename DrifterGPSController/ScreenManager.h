#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include "Screen.h"

// CustomDisplay class that extends Adafruit_SSD1306
class ScreenManager {
private:
  
  Adafruit_SSD1306& display;
public:
  int8_t currentScreen; // Index of the displayed screen
  uint8_t numberOfScreens; // Number of held screens, cannot exceed 4
  Screen* screens[4]; 
  // Constructor
  ScreenManager(Adafruit_SSD1306& display) : display(display), numberOfScreens(0), currentScreen(-1), screens{nullptr} {}
  void draw(); // Draws the tabs, current screen, and blinker
  void update(); // Updates the buffer and draws the the screen
  void drawTabs(); // Draws the tabs at the top
  void drawScreen(); // Draws the current screen
  void drawBlinker(); // Draws the blinking light in the corner
  void addScreen(Screen* screen); // Adds a screen at the end
  void addScreen(Screen* screen, uint8_t index); // Adds a screen at the given index
  void removeScreen(uint8_t index); // Removes the screen at the given index
  void switchScreen(uint8_t index); // Switches the displayed screen to the given index
};

#endif