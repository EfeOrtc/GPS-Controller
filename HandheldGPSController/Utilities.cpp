#include "Utilities.h"
// Formatting functions
String Utilities::formatNumber(double number, int8_t wholeDigits, int8_t decimalDigits, bool sign) {
  if (sign) {
    String output = formatNumber(abs(number), wholeDigits, decimalDigits);
    if (number < 0) {
      return "-" + output;
    } else {
      return "+" + output;
    }
  } else {
    return Utilities::formatNumber(number, wholeDigits, decimalDigits);
  }
}
String Utilities::formatNumber(double number, int8_t wholeDigits, int8_t decimalDigits) {
  String output = String((int)floor(abs(number)));
  if (number < 0) {
    output = "-" + output;
  }
  // Whole part
  if (wholeDigits >= 0) {
    while (output.length() < wholeDigits) {
      output = "0" + output;
    }
  }
  // Fractional part
  output += String(number, decimalDigits).substring(String(number, decimalDigits).indexOf("."));
  return output;
}
String Utilities::formatDate(const TinyGPSDate &date) {  // 00-00-0000
  return Utilities::formatNumber(date.month(), 2, 0) + "-" + Utilities::formatNumber(date.day(), 2, 0) + "-" + Utilities::formatNumber(date.year(), 4, 0);
}
String Utilities::formatTime(const TinyGPSTime &time) {  //00:00:00
  return Utilities::formatNumber(time.hour(), 2, 0) + ":" + Utilities::formatNumber(time.minute(), 2, 0) + ":" + Utilities::formatNumber(time.second(), 2, 0);  // + "." + Utilities::formatNumber(time.centisecond(), 2, 0);
}
String Utilities::formatDateTime(const TinyGPSDate &date, const TinyGPSTime &time) {  //00-00-0000-00:00:00
  return Utilities::formatDate(date) + "-" + Utilities::formatTime(time);
}
String Utilities::formatDateTimeISO8601(const TinyGPSDate &date, const TinyGPSTime &time) {
  return Utilities::formatNumber(date.year(), 4, 0) + "-" + Utilities::formatNumber(date.month(), 2, 0) + "-" + Utilities::formatNumber(date.day(), 2, 0) + "T" + Utilities::formatTime(time) + "Z";
}

void Utilities::printToFile(const String output, String &fileName) {
  File dataFile = SD.open(fileName, FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.print(output + "\n");
  } else {
    Serial.println("*** File \"" + fileName + "\" unavailable ***");
  }
  dataFile.close();
}
void Utilities::printlnToFile(const String output, String &fileName) {
  Utilities::printToFile(output + "\n", fileName);
}
String Utilities::getGPXtrkpt(const TinyGPSDate &date, const TinyGPSTime &time, const TinyGPSLocation &location, const TinyGPSAltitude &altitude) {
  String output = "   <trkpt lat=\"" + Utilities::formatNumber(location.lat(), 2, 7) + "\" lon=\"" + Utilities::formatNumber(location.lng(), 2, 7) + "\">\n";
  output += "    <ele>" + Utilities::formatNumber(altitude.meters(), 0, 1) + "</ele>\n";
  output += "    <time>" + Utilities::formatDateTimeISO8601(date, time) + "</time>\n";
  output += "   </trkpt>";
  return output;
}
// Calculation functions
double Utilities::timeBetween(const TinyGPSTime &time, const TinyGPSTime &previousTime) {
  return (((time.hour() * 3600 + time.minute() * 60 + time.second()) - (previousTime.hour() * 3600 + previousTime.minute() * 60 + previousTime.second())) + 8640000) % 8640000;
}
double Utilities::degreesToRadians(const double degrees) {
  return degrees * (M_PI / 180);
}
int Utilities::freeMemory() {
#ifdef __arm__
  // For ARM-based boards (e.g., Arduino Due)
  extern "C" char *sbrk(int incr);
  char top;  // Stack pointer
  return &top - reinterpret_cast<char *>(sbrk(0));
#elif defined(__AVR__)
  // For AVR-based boards
  extern char __heap_start, *__brkval;
  char top;  // Stack pointer
  if (__brkval == 0) {
    // If no heap allocation, calculate free memory from stack to heap start
    return &top - &__heap_start;
  } else {
    // If heap allocation exists, calculate free memory from stack to heap end
    return &top - __brkval;
  }
#else
  // Unknown architecture, free memory calculation unsupported
  return -1;
#endif
}