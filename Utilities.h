#ifndef UTILITIES_H
#define UTILITIES_H

#include <Arduino.h>
#include <SD.h>
#include <TinyGPSPLUS.h>
#include <Adafruit_SSD1306.h>
//#include "TerminalDisplay.h"

class Utilities {
public:

  static String formatNumber(double number, uint8_t wholeDigits, uint8_t decimalDigits, bool sign); // Returns the formatted number with a + for positive numbers
  static String formatNumber(double number, uint8_t wholeDigits, uint8_t decimalDigits); // Returns the formatted number
  static String formatDate(TinyGPSDate &date); // Returns date in MM-DD-YYYY / 01-31-2000
  static String formatTime(TinyGPSTime &time); // Returns time in HH:MM:SS / 23:59:59
  static String formatDateTime(TinyGPSDate &date, TinyGPSTime &time); // Returns date and time in MM-DD-YYYY-HH:MM:SS / 01-31-2000-23:59:59
  static String formatDateTimeISO8601(TinyGPSDate &date, TinyGPSTime &time); // Returns date and time in ISO 8601 format: YYYY-MM-DDTHH:MM:SSZ / 2000-01-31T23:59:59Z

  static void smartPrint(String output, String &fileName); // Prints to both the Serial and the given file
  static void printToFile(String output, String &fileName); // Prints the string to the given file
  static void printlnToFile(String output, String &fileName); // Prints the string to the given file on a new line
  static String getGPXtrkpt(TinyGPSDate &date, TinyGPSTime &time, TinyGPSLocation &location, TinyGPSAltitude &altitude); // Returns a string of the current GPX trkpt segment

  static double timeBetween(TinyGPSTime &time, TinyGPSTime &previousTime); // Returns time between the two time stamps in seconds
  static double degreesToRadians(double degrees); // Converts radians to degrees
  static int freeMemory(); // Returns the amount of free memory
};

#endif