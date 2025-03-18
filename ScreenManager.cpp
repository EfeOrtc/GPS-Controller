#include "ScreenManager.h"
#include "Utilities.h"

void ScreenManager::draw() {
  drawTabs();
  drawScreen();
  drawBlinker();
}
void ScreenManager::drawTabs() {
  for (int8_t i = 0; i < numberOfScreens; i++) {
    screens[i]->drawTab(i, currentScreen == i);
  }
}
void ScreenManager::drawScreen() {
  if(numberOfScreens != 0) {
    screens[currentScreen]->draw();
  } else {
    
  }
}
void ScreenManager::drawBlinker() {
  Serial.println("Free Memory: " + String(Utilities::freeMemory()));
  Serial.println("111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111");
  display.drawRect(110, 0, 10, 10, 0);
  Serial.println("222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222");
  display.drawRect(112, 1, 14, 14, 0);
  Serial.println("333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333");
  if((millis() / 1000) % 2 == 0) {
    display.fillRect(113, 2, 12, 12, 0);

  } else {
    display.fillRect(113, 2, 12, 12, 0); // SSD1306_BLACK
  }
}
void ScreenManager::addScreen(Screen* screen) {
  if(numberOfScreens < 4) {
    screens[numberOfScreens] = screen;
    numberOfScreens++;
    if(currentScreen == -1) {
      currentScreen = numberOfScreens - 1;
    }
  }
}
void ScreenManager::addScreen(Screen* screen, uint8_t index) {
  if(numberOfScreens < 4 && index < numberOfScreens) {
    for(int i = numberOfScreens; i > index; i--) {
      screens[i] = screens[i - 1];
    }
    screens[index] = screen;
    if(currentScreen == -1) {
      currentScreen = numberOfScreens;
    }
  }
}
void ScreenManager::removeScreen(uint8_t index) {
  if(index < numberOfScreens) {
    for(int i = index; i < numberOfScreens - 1; i++) {
      screens[i] = screens[i + 1];
    }
    screens[numberOfScreens] = nullptr;
  }
}
void ScreenManager::switchScreen(uint8_t index) {
  if(index < numberOfScreens) {
    currentScreen = index;
  } else {
    currentScreen = numberOfScreens - 1;
  }
}