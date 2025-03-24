#include "ActivityMonitor.h"

// Updating
void ActivityMonitor::update() {
  location.update();
  speed.update();
  distance.update();
  navigation.update();
  elevation.update();
  timer.update();
  date.update();
}
void ActivityMonitor::begin(String fileNameGPX) {
  fileName = fileNameGPX;
  timer.startTime = gps.time;
  distance.previousLocation = gps.location;
  printGPXStart();
  update();
}
void ActivityMonitor::printGPXStart() {
  String fileStart = "<gpx xmlns=\"http://www.topografix.com/GPX/1/1\" creator=\"ArduinoGPS\" version=\"1.1\">\n";
  fileStart += " <metadata>\n";
  fileStart += "  <time>" + Utilities::formatDateTimeISO8601(gps.date, gps.time) + "</time>\n";
  fileStart += "  <name>Activity from " + Utilities::formatDate(gps.date) + " at " + Utilities::formatTime(gps.time) + "</time>\n";
  fileStart += " </metadata>\n";
  fileStart += " <trk>\n";
  fileStart += String("  <type>") + "running" + "</type>\n";
  fileStart += "  <trkseg>";
  Utilities::printToFile(fileStart, fileName);
}
void ActivityMonitor::printGPXtrkpt() {
  String output = String("   <trkpt lat=\"") + Utilities::formatNumber(location.latitude(), 2, 7) + "\" lon=\"" + Utilities::formatNumber(location.longitude(), 2, 7) + "\">\n";
  output += "    <ele>" + Utilities::formatNumber(elevation.elevation(), 0, 1) + "</ele>\n";
  output += "    <time>" + Utilities::formatDateTimeISO8601(date.date(), date.time()) + "</time>\n";
  output += "   </trkpt>";
  Utilities::printToFile(output, fileName);
}

// Location Module Implementation
void Location::update() {
  if(gps.location.isUpdated()) {
    updated = true;
    lastUpdateTime = millis();
    rawLatitude = gps.location.lat();
    rawLongitude = gps.location.lng();
  }
}

// Speed Module Implementation
void Speed::update() {
  if(gps.speed.isUpdated()) {
    updated = true;
    lastUpdateTime = millis();
    currentSpeedkmph = gps.speed.kmph();
    if(currentSpeedkmph > rawMaxSpeedkmph) {
      rawMaxSpeedkmph = currentSpeedkmph;
    }
    for(int8_t i = 9; i > 0; i--) {
      previousSpeeds[i] = previousSpeeds[i - 1];
    }
    previousSpeeds[0] = currentSpeedkmph;
  }
}

// Distance Module Implementation
void Distance::update() {
  updated = true;
  lastUpdateTime = millis();

  TinyGPSLocation currentLocation = gps.location;
  rawDistancekm += gps.distanceBetween(currentLocation.lat(), currentLocation.lng(), previousLocation.lat(), previousLocation.lng()) / 1000;
  previousLocation = currentLocation;
}

// Navigation Module Implementation
void Navigation::update() {
  updated = true;
  lastUpdateTime = millis();

  rawHeading = gps.course.deg();
}

// Elevation Module Implementation
void Elevation::update() {
  updated = true;
  lastUpdateTime = millis();

  rawElevation = gps.altitude.meters();
}

// Timer Module Implementation
void Timer::update() {
  updated = true;
  lastUpdateTime = millis();
}

// Date Module Implementation
void Date::update() {
  updated = true;
  lastUpdateTime = millis();

  currentTime = gps.time;
  currentDate = gps.date;
}