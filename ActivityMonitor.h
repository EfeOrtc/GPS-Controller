#ifndef ACTIVITYMONITOR_H
#define ACTIVITYMONITOR_H

#include <Arduino.h>
#include <TinyGPSPLUS.h>
#include "Utilities.h"

// Modules for holding values
struct Location {
  friend class ActivityMonitor;
private:
  TinyGPSPlus& gps;
public:
  Location(TinyGPSPlus& gps) : gps(gps) {}
  double latitude();
  double longitude();
};

struct Speed {
  friend class ActivityMonitor;
private:
  TinyGPSPlus& gps;
  double& totalDistance;
  TinyGPSTime& startTime;
  double previousSpeeds[10] = {0};
  double maxSpeedkmph;
public:
  Speed(TinyGPSPlus& gps, double& totalDistance, TinyGPSTime& startTime) : gps(gps), totalDistance(totalDistance), startTime(startTime), maxSpeedkmph(0), previousSpeeds({0}) {}
  // Speeds
  double averagespeedkmph();
  double averagespeedmph();
  double speedkmph();
  double speedmph();
  double maxspeedkmph();
  double maxspeedmph();
  double rollingspeedkmph();
  double rollingspeedmph();
  // Paces
  String averagepaceminpkm();
  String averagepaceminpmi();
  String paceminpkm();
  String paceminpmi();
  String rollingpaceminpkm();
  String rollingpaceminpmi();
};

struct Distance {
  friend class ActivityMonitor;
private:
  TinyGPSPlus& gps;
  TinyGPSLocation previousLocation;
public:
  double totalDistancekm;
  Distance(TinyGPSPlus& gps) : gps(gps), totalDistancekm(0) {}
  double distancekm();
  double distancemi();
};

struct Navigation {
  friend class ActivityMonitor;
private:
  TinyGPSPlus& gps;
public:
  Navigation(TinyGPSPlus& gps) : gps(gps) {}
  double heading();
};

struct Elevation {
  friend class ActivityMonitor;
private:
  TinyGPSPlus& gps;
public:
  Elevation(TinyGPSPlus& gps) : gps(gps) {}
  double elevation();
};

struct Timer {
  friend class ActivityMonitor;
private:
  TinyGPSPlus& gps;
public:
  TinyGPSTime& startTime;
  Timer(TinyGPSPlus& gps) : gps(gps) {}
  double elapsedtime();
};

struct Date {
  friend class ActivityMonitor;
private:
  TinyGPSPlus& gps;
public:
  Date(TinyGPSPlus gps) : gps(gps) {}
  String date();
  String timeofday();
  String timeofdayseconds();
};

class ActivityMonitor {
private:
  TinyGPSPlus& gps;
public:
  ActivityMonitor(TinyGPSPlus& gps) : gps(gps), location(gps), distance(gps), navigation(gps), elevation(gps), date(gps), timer(gps), speed(gps, distance.totalDistancekm, timer.startTime) {}
  // Different Modules
  Location location;
  Speed speed;
  Distance distance;
  Navigation navigation;
  Elevation elevation;
  Timer timer;
  Date date;
  void update(); // Updates all the values
};

#endif