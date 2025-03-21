#include "Valuedisplay.h"
void ValueDisplay::addValue(String title, String (ValueDisplay::*value)()) {
  if(numValues < 6) {
    titles[numValues] = title;
    values[numValues] = value;
    numValues++;
  }
}
void ValueDisplay::addValue(String title, String (ValueDisplay::*value)(), uint8_t index) {
  if(numValues < 6 && index < numValues) {
    for(int i = numValues; i > index; i--) {
      titles[i] = titles[i - 1];
      values[i] = values[i - 1];
    }
    titles[index] = title;
    values[index] = value;
  }
}
void ValueDisplay::removeValue(uint8_t index) {
  if(index < numValues) {
    for(int i = index; i < numValues - 1; i++) {
      titles[i] = titles[i + 1];
      values[i] = values[i + 1];
    }
    titles[numValues] = "";
    values[numValues] = nullptr;
  }
}

/*
ValueDisplay::ValueDisplay(Adafruit_SSD1306& display, TinyGPSPlus gps) : Screen(display, "DIS"), am(gps) { // titles is the titles, delimited by a comma
  int8_t numTitles = 0;
  while(titleList.indexOf(",") >= 0 && numTitles <= 6) {
    numTitles++;
    String curTitle = titleList.substring(0, titleList.indexOf(","));
    if(curTitle == "latitude") {
      values[numTitles - 1] = &ValueDisplay::getlatitude;
      titles[numTitles - 1] = "Latitude";
    } else if(curTitle == "longitude") {
      values[numTitles - 1] = &ValueDisplay::getlongitude;
      titles[numTitles - 1] = "Longitude";
    } else if(curTitle == "averagespeedkmph") {
      values[numTitles - 1] = &ValueDisplay::getaveragespeedkmph;
      titles[numTitles - 1] = "Avg Speed";
    } else if(curTitle == "averagespeedmph") {
      values[numTitles - 1] = &ValueDisplay::getaveragespeedmph;
      titles[numTitles - 1] = "Avg Speed";
    } else if(curTitle == "speedkmph") {
      values[numTitles - 1] = &ValueDisplay::getspeedkmph;
      titles[numTitles - 1] = "Speed";
    } else if(curTitle == "speedmph") {
      values[numTitles - 1] = &ValueDisplay::getspeedmph;
      titles[numTitles - 1] = "Speed";
    } else if(curTitle == "maxspeedkmph") {
      values[numTitles - 1] = &ValueDisplay::getmaxspeedkmph;
      titles[numTitles - 1] = "Max Speed";
    } else if(curTitle == "maxspeedmph") {
      values[numTitles - 1] = &ValueDisplay::getmaxspeedmph;
      titles[numTitles - 1] = "Max Speed";
    } else if(curTitle == "rollingspeedkmph") {
      values[numTitles - 1] = &ValueDisplay::getrollingspeedkmph;
      titles[numTitles - 1] = "Speed";
    } else if(curTitle == "rollingspeedmph") {
      values[numTitles - 1] = &ValueDisplay::getrollingspeedmph;
      titles[numTitles - 1] = "Speed";
    } else if(curTitle == "averagepaceminpkm") {
      values[numTitles - 1] = &ValueDisplay::getaveragepaceminpkm;
      titles[numTitles - 1] = "Avg Pace";
    } else if(curTitle == "averagepaceminpmi") {
      values[numTitles - 1] = &ValueDisplay::getaveragepaceminpmi;
      titles[numTitles - 1] = "Avg Pace";
    } else if(curTitle == "paceminpkm") {
      values[numTitles - 1] = &ValueDisplay::getpaceminpkm;
      titles[numTitles - 1] = "Pace";
    } else if(curTitle == "paceminpmi") {
      values[numTitles - 1] = &ValueDisplay::getpaceminpmi;
      titles[numTitles - 1] = "Pace";
    } else if(curTitle == "rollingpaceminpkm") {
      values[numTitles - 1] = &ValueDisplay::getrollingpaceminpkm;
      titles[numTitles - 1] = "Pace";
    } else if(curTitle == "rollingpaceminpmi") {
      values[numTitles - 1] = &ValueDisplay::getrollingpaceminpmi;
      titles[numTitles - 1] = "Pace";
    } else if(curTitle == "distancekm") {
      values[numTitles - 1] = &ValueDisplay::getdistancekm;
      titles[numTitles - 1] = "Distance";
    } else if(curTitle == "distancemi") {
      values[numTitles - 1] = &ValueDisplay::getdistancemi;
      titles[numTitles - 1] = "Distance";
    } else if(curTitle == "heading") {
      values[numTitles - 1] = &ValueDisplay::getheading;
      titles[numTitles - 1] = "Heading";
    } else if(curTitle == "elevation") {
      values[numTitles - 1] = &ValueDisplay::getelevation;
      titles[numTitles - 1] = "Elevation";
    } else if(curTitle == "elapsedtime") {
      values[numTitles - 1] = &ValueDisplay::getelapsedtime;
      titles[numTitles - 1] = "Time";
    } else if(curTitle == "date") {
      values[numTitles - 1] = &ValueDisplay::getdate;
      titles[numTitles - 1] = "Date";
    } else if(curTitle == "timeofday") {
      values[numTitles - 1] = &ValueDisplay::gettimeofday;
      titles[numTitles - 1] = "Time Day";
    } else if(curTitle == "timeofdayseconds") {
      values[numTitles - 1] = &ValueDisplay::gettimeofdayseconds;
      titles[numTitles - 1] = "Time Day";
    }
  }
  for(int8_t i = 0; i < 6; i++) {
    //terminalLines[i] = "";
  }
}
*/
void ValueDisplay::draw() {
  //am.update();
  display.fillRect(0, 16, 128, 48, SSD1306_BLACK);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setTextWrap(false);
  for(int8_t i = 0; i < numValues; i++) {
    display.setCursor(0, 16 + 8 * (i));
    display.print(titles[i] + ": " + (this->*values[i])());
  }
}

String ValueDisplay::getlatitude () {
  return Utilities::formatNumber(am.location.latitude(), 2, 7);
}
String ValueDisplay::getlongitude () {
  return Utilities::formatNumber(am.location.longitude(), 2, 7);
}
String ValueDisplay::getaveragespeedkmph () {
  return Utilities::formatNumber(am.speed.averagespeedkmph(), -1, 2);
}
String ValueDisplay::getaveragespeedmph () {
  return Utilities::formatNumber(am.speed.averagespeedmph(), -1, 2);
}
String ValueDisplay::getspeedkmph () {
  return Utilities::formatNumber(am.speed.speedkmph(), -1, 2);
}
String ValueDisplay::getspeedmph () {
  return Utilities::formatNumber(am.speed.speedmph(), -1, 2);
}
String ValueDisplay::getmaxspeedkmph () {
  return Utilities::formatNumber(am.speed.maxspeedkmph(), -1, 2);
}
String ValueDisplay::getmaxspeedmph () {
  return Utilities::formatNumber(am.speed.maxspeedmph(), -1, 2);
}
String ValueDisplay::getrollingspeedkmph () {
  return Utilities::formatNumber(am.speed.rollingspeedkmph(), -1, 2);
}
String ValueDisplay::getrollingspeedmph () {
  return Utilities::formatNumber(am.speed.rollingspeedmph(), -1, 2);
}
String ValueDisplay::getaveragepaceminpkm () {
  double pace = am.speed.averagepaceminpkm();
  return String(floor(pace), 0) + ":" + Utilities::formatNumber((pace - floor(pace)) * 60, 2, 0);
}
String ValueDisplay::getaveragepaceminpmi () {
  double pace = am.speed.averagepaceminpmi();
  return String(floor(pace), 0) + ":" + Utilities::formatNumber((pace - floor(pace)) * 60, 2, 0);
}
String ValueDisplay::getpaceminpkm () {
  double pace = am.speed.paceminpkm();
  return String(floor(pace), 0) + ":" + Utilities::formatNumber((pace - floor(pace)) * 60, 2, 0);
}
String ValueDisplay::getpaceminpmi () {
  double pace = am.speed.paceminpmi();
  return String(floor(pace), 0) + ":" + Utilities::formatNumber((pace - floor(pace)) * 60, 2, 0);
}
String ValueDisplay::getrollingpaceminpkm () {
  double pace = am.speed.rollingpaceminpkm();
  return String(floor(pace), 0) + ":" + Utilities::formatNumber((pace - floor(pace)) * 60, 2, 0);
}
String ValueDisplay::getrollingpaceminpmi () {
  double pace = am.speed.rollingpaceminpkm();
  return String(floor(pace), 0) + ":" + Utilities::formatNumber((pace - floor(pace)) * 60, 2, 0);
}
String ValueDisplay::getdistancekm () {
  return Utilities::formatNumber(am.distance.distancekm(), -1, 2);
}
String ValueDisplay::getdistancemi () {
  Serial.println(String(am.distance.distancemi(), 2));
  return Utilities::formatNumber(am.distance.distancemi(), -1, 2);
}
String ValueDisplay::getheading () {
  return Utilities::formatNumber(am.navigation.heading(), -1, 2);
}
String ValueDisplay::getelevation () {
  return Utilities::formatNumber(am.elevation.elevation(), -1, 2);
}
String ValueDisplay::getelapsedtime () {
  int elapsedTime = am.timer.elapsedtime(); // In seconds
  String output = Utilities::formatNumber(elapsedTime / 3600, 2, 0) + ":";
  elapsedTime %= 3600;
  output += Utilities::formatNumber(elapsedTime / 60, 2, 0) + ":";
  elapsedTime %= 60;
  output += Utilities::formatNumber(elapsedTime, 2, 0);
  return output;
}
String ValueDisplay::getdate () {
  return Utilities::formatDate(am.date.date());
}
String ValueDisplay::gettimeofday () {
  return Utilities::formatTime(am.date.time()).substring(0, 5);
}
String ValueDisplay::gettimeofdayseconds() {
  TinyGPSTime time = am.date.time();
  return Utilities::formatTime(am.date.time());
}