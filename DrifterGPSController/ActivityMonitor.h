#ifndef ACTIVITYMONITOR_H
#define ACTIVITYMONITOR_H

#include <Arduino.h>
#include <TinyGPSPlus.h>
#include "Utilities.h"

// Modules for holding values
struct Location {
  friend class ActivityMonitor;
private:
  TinyGPSPlus& gps;
  bool updated;
  uint32_t lastUpdateTime;
  void update();
  double rawLatitude, rawLongitude;
public:
  Location(TinyGPSPlus& gps) : gps(gps), updated(false) {}
  uint32_t age() {return millis() - lastUpdateTime;}
  double isUpdated() {return updated;}
  double latitude() {updated = false; return rawLatitude;}
  double longitude() {updated = false; return rawLongitude;}
};

struct Speed {
  friend class ActivityMonitor;
private:
  TinyGPSPlus& gps;
  bool updated;
  uint32_t lastUpdateTime;
  void update();

  double previousSpeeds[10];
  double rawMaxSpeedkmph;
  double currentSpeedkmph;
  double& rawDistancekm;
  TinyGPSTime& startTime;
  TinyGPSTime& currentTime;
public:
  Speed(TinyGPSPlus& gps, double& rawDistancekm, TinyGPSTime& startTime, TinyGPSTime& currentTime) : gps(gps), rawDistancekm(rawDistancekm), startTime(startTime), currentTime(currentTime), rawMaxSpeedkmph(0), currentSpeedkmph(0), previousSpeeds({0}) {}
  uint32_t age() {return millis() - lastUpdateTime;}
  double isUpdated() {return updated;}
  // Speeds
  double averagespeedkmph() {double time = Utilities::timeBetween(currentTime, startTime) / 3600; return time != 0 ? rawDistancekm/time : 0;}
  double averagespeedmph() {return averagespeedkmph()/1.609;}
  double speedkmph() {return currentSpeedkmph;}
  double speedmph() {return currentSpeedkmph / 1.609;}
  double maxspeedkmph() {return rawMaxSpeedkmph;}
  double maxspeedmph() {return rawMaxSpeedkmph/1.609;}
  double rollingspeedkmph() {double avg = 0;for(uint8_t i = 0; i < 10; i++) {avg += previousSpeeds[i];} avg /= 10.0; return avg;}
  double rollingspeedmph() {return rollingspeedkmph()/1.609;}
  // Paces
  double averagepaceminpkm() {return rawDistancekm != 0 ? (Utilities::timeBetween(currentTime, startTime) / 60.0) / rawDistancekm : 0;}//{if(totalDistance != 0) {double minpkm = (Utilities::timeBetween(gps.time, startTime) / 60.0) / totalDistance; return Utilities::formatNumber(floor(minpkm), -1, 0) + ":" + Utilities::formatNumber((minpkm - floor(minpkm)) * 60.0, 2, 0);} return "--:--";}
  double averagepaceminpmi() {return rawDistancekm != 0 ? (Utilities::timeBetween(currentTime, startTime) / 60.0) / (rawDistancekm / 1.609) : 0;}//{if(totalDistance != 0) {double minpkm = (Utilities::timeBetween(gps.time, startTime) / 60.0) / (totalDistance / 1.609); return Utilities::formatNumber(floor(minpkm), -1, 0) + ":" + Utilities::formatNumber((minpkm - floor(minpkm)) * 60.0, 2, 0);} return "--:--";}
  double paceminpkm() {double speed = currentSpeedkmph; return speed != 0 ? 60.0/speed : 0;}//{double speed = gps.speed.kmph(); if(speed != 0) {double minpkm = 60.0/speed; return Utilities::formatNumber(floor(minpkm), -1, 0) + ":" + Utilities::formatNumber((minpkm - floor(minpkm)) * 60.0, 2, 0);} return "--:--";}
  double paceminpmi() {double speed = currentSpeedkmph / 1.609; return speed != 0 ? 60.0/speed : 0;}//{double speed = gps.speed.mph(); if(speed != 0) {double minpmi = 60.0/speed; return Utilities::formatNumber(floor(minpmi), -1, 0) + ":" + Utilities::formatNumber((minpmi - floor(minpmi)) * 60.0, 2, 0);} return "--:--";}
  double rollingpaceminpkm() {double avg = 0; for(uint8_t i = 0; i < 10; i++) {avg += previousSpeeds[i];} avg /= 10.0; return avg != 0 ? 60.0/avg : 0;}//if(avg != 0) {double minpkm = 60.0/avg; return Utilities::formatNumber(floor(minpkm), -1, 0) + ":" + Utilities::formatNumber((minpkm - floor(minpkm)) * 60.0, 2, 0);} return "--:--";}
  double rollingpaceminpmi() {double avg = 0; for(uint8_t i = 0; i < 10; i++) {avg += previousSpeeds[i];} avg /= 10.0; avg /= 1.609; return avg != 0 ? 60.0/avg : 0;}//if(avg != 0) {double minpkm = 60.0/avg; return Utilities::formatNumber(floor(minpkm), -1, 0) + ":" + Utilities::formatNumber((minpkm - floor(minpkm)) * 60.0, 2, 0);} return "--:--";}
};

struct Distance {
  friend class ActivityMonitor;
private:
  TinyGPSPlus& gps;
  bool updated;
  uint32_t lastUpdateTime;
  void update();

  double rawDistancekm;
  TinyGPSLocation previousLocation;
public:
  Distance(TinyGPSPlus& gps) : gps(gps), rawDistancekm(0) {}
  uint32_t age() {return millis() - lastUpdateTime;}
  double isUpdated() {return updated;}

  double distancekm() {return rawDistancekm;}
  double distancemi() {return rawDistancekm / 1.609;}
};

struct Navigation {
  friend class ActivityMonitor;
private:
  TinyGPSPlus& gps;
  bool updated;
  uint32_t lastUpdateTime;
  void update();
  double rawHeading;
public:
  Navigation(TinyGPSPlus& gps) : gps(gps) {}
  uint32_t age() {return millis() - lastUpdateTime;}
  double isUpdated() {return updated;}

  double heading() {return rawHeading;}
};

struct Elevation {
  friend class ActivityMonitor;
private:
  TinyGPSPlus& gps;
  bool updated;
  uint32_t lastUpdateTime;
  void update();

  double rawElevation;
public:
  Elevation(TinyGPSPlus& gps) : gps(gps) {}
  uint32_t age() {return millis() - lastUpdateTime;}
  double isUpdated() {return updated;}

  double elevation() {return rawElevation;}
};

struct Timer {
  friend class ActivityMonitor;
private:
  TinyGPSPlus& gps;
  bool updated;
  uint32_t lastUpdateTime;
  void update();

  TinyGPSTime startTime;
  TinyGPSTime& currentTime;
public:
  Timer(TinyGPSPlus& gps, TinyGPSTime &currentTime) : gps(gps), currentTime(currentTime) {}
  uint32_t age() {return millis() - lastUpdateTime;}
  double isUpdated() {return updated;}

  double elapsedtime() {return Utilities::timeBetween(currentTime, startTime);}
};

struct Date {
  friend class ActivityMonitor;
private:
  TinyGPSPlus& gps;
  bool updated;
  uint32_t lastUpdateTime;
  void update();

  TinyGPSTime currentTime;
  TinyGPSDate currentDate;
public:
  Date(TinyGPSPlus& gps) : gps(gps) {}
  uint32_t age() {return millis() - lastUpdateTime;}
  double isUpdated() {return updated;}

  TinyGPSTime time() {return currentTime;}
  TinyGPSDate date() {return currentDate;}
};

class ActivityMonitor {
private:
  TinyGPSPlus& gps;
  String fileName;
public:
  ActivityMonitor(TinyGPSPlus& gps) : gps(gps), location(gps), distance(gps), navigation(gps), elevation(gps), date(gps), timer(gps, date.currentTime), speed(gps, distance.rawDistancekm, timer.startTime, date.currentTime) {}
  // Different Modules
  Location location;
  Speed speed;
  Distance distance;
  Navigation navigation;
  Elevation elevation;
  Timer timer;
  Date date;
  void update(); // Updates all the values
  void begin(String fileName);
  void printGPXStart();
  void printGPXtrkpt();
  String getFileName() {return fileName;}
};

#endif