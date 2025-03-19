#include "ActivityMonitor.h"

// Updating
void ActivityMonitor::update() {
  // Updating speed
  double currentSpeed = gps.speed.kmph();
  if(currentSpeed > speed.maxSpeedkmph) {
    speed.maxSpeedkmph = currentSpeed;
  }
  for(int8_t i = 9; i > 0; i--) {
    speed.previousSpeeds[i] = speed.previousSpeeds[i - 1];
  }
  speed.previousSpeeds[0] = currentSpeed;
  // Updating distance
  TinyGPSLocation currentLocation = gps.location;
  //distance.totalDistancekm += gps.distanceBetween(currentLocation, distance.previousLocation);
  distance.totalDistancekm += gps.distanceBetween(currentLocation.lat(), currentLocation.lng(), distance.previousLocation.lat(), distance.previousLocation.lng());
  distance.previousLocation = currentLocation;

}
void ActivityMonitor::begin() {
  timer.startTime = gps.time;
  
}
// Position Module Implementation
double Location::latitude() {
  //return Utilities::formatNumber(gps.location.lat(), 2, 7);
  return gps.location.lat();
}

double Location::longitude() {
  //return Utilities::formatNumber(gps.location.lng(), 2, 7);
  return gps.location.lng();
}

// Speed Module Implementation
double Speed::averagespeedkmph() {
  double time = Utilities::timeBetween(gps.time, startTime) / 3600;
  if(time != 0) {
    //return Utilities::formatNumber(totalDistance / time, -1, 2);
    return totalDistance / time;
  } else {
    //return "-.--";
    return 0;
  }
}

double Speed::averagespeedmph() {
  double time = Utilities::timeBetween(gps.time, startTime) / 3600;
  if(time != 0) {
    //return Utilities::formatNumber((totalDistance / 1.609) / time, -1, 2);
  } else {
    //return "-.--";
  }
}
double Speed::speedkmph() {
  //return Utilities::formatNumber(gps.speed.kmph(), -1, 2);
  return gps.speed.kmph();
}

double Speed::speedmph() {
  //return Utilities::formatNumber(gps.speed.mph(), -1, 2);
  return gps.speed.mph();
}

double Speed::maxspeedkmph() {
  //return Utilities::formatNumber(maxSpeedkmph, -1, 2);
  return maxSpeedkmph;
}

double Speed::maxspeedmph() {
  //return Utilities::formatNumber(maxSpeedkmph / 1.609, -1, 2);
  return maxSpeedkmph/1.609;
}

double Speed::rollingspeedkmph() {
  double avg = 0;
  for(uint8_t i = 0; i < 10; i++) {
    avg += previousSpeeds[i];
  }
  avg /= 10.0;
  //return Utilities::formatNumber(avg, -1, 2);
  return avg;
}

double Speed::rollingspeedmph() {
  double avg = 0;
  for(uint8_t i = 0; i < 10; i++) {
    avg += previousSpeeds[i];
  }
  avg /= 10.0;
  avg /= 1.609;
  //return Utilities::formatNumber(avg, -1, 2);
  return avg;
}

// Paces
String Speed::averagepaceminpkm() {
  if(totalDistance != 0) {
    double minpkm = (Utilities::timeBetween(gps.time, startTime) / 60.0) / totalDistance;
    return Utilities::formatNumber(floor(minpkm), -1, 0) + ":" + Utilities::formatNumber((minpkm - floor(minpkm)) * 60.0, 2, 0);
  } else {
    return "--:--";
  }
}

String Speed::averagepaceminpmi() {
  if(totalDistance != 0) {
    double minpmi = (Utilities::timeBetween(gps.time, startTime) / 60.0) / (totalDistance / 1.609);
    return Utilities::formatNumber(floor(minpmi), -1, 0) + ":" + Utilities::formatNumber((minpmi - floor(minpmi)) * 60.0, 2, 0);
  } else {
    return "--:--";
  }
}
String Speed::paceminpkm() {
  double speed = gps.speed.kmph();
  if(speed != 0) {
    double minpkm = 60.0/speed;
    return Utilities::formatNumber(floor(minpkm), -1, 0) + ":" + Utilities::formatNumber((minpkm - floor(minpkm)) * 60.0, 2, 0);
  } else {
    return "--:--";
  }
}

String Speed::paceminpmi() {
  double speed = gps.speed.mph();
  if(speed != 0) {
    double minpmi = 60.0/speed;
    return Utilities::formatNumber(floor(minpmi), -1, 0) + ":" + Utilities::formatNumber((minpmi - floor(minpmi)) * 60.0, 2, 0);
  } else {
    return "--:--";
  }
}

String Speed::rollingpaceminpkm() {
  double avg = 0;
  for(uint8_t i = 0; i < 10; i++) {
    avg += previousSpeeds[i];
  }
  avg /= 10.0;
  if(avg != 0) {
    double minpkm = 60.0/avg;
    return Utilities::formatNumber(floor(minpkm), -1, 0) + ":" + Utilities::formatNumber((minpkm - floor(minpkm)) * 60.0, 2, 0);
  } else {
    return "--:--";
  }
}

String Speed::rollingpaceminpmi() {
  double avg = 0;
  for(uint8_t i = 0; i < 10; i++) {
    avg += previousSpeeds[i];
  }
  avg /= 10.0;
  avg /= 1.609;
  if(avg != 0) {
    double minpmi = 60.0/avg;
    return Utilities::formatNumber(floor(minpmi), -1, 0) + ":" + Utilities::formatNumber((minpmi - floor(minpmi)) * 60.0, 2, 0);
  } else {
    return "--:--";
  }
}

// Distance Module Implementation
double Distance::distancekm() {
  //return Utilities::formatNumber(totalDistancekm, -1, 2);
  return totalDistancekm;
}

double Distance::distancemi() {
  //return Utilities::formatNumber(totalDistancekm / 1.609, -1, 2);
  return totalDistancekm/1.609;
}

// Navigation Module Implementation
double Navigation::heading() {
  //return Utilities::formatNumber(gps.course.deg(), -1, 2);
  return gps.course.deg();
}

// Elevation Module Implementation
double Elevation::elevation() {
  //return Utilities::formatNumber(gps.altitude.meters(), -1, 2);
  return gps.altitude.meters();
}

// Timer Module Implementation
double Timer::elapsedtime() {
  /*
  int timeBetween = Utilities::timeBetween(gps.time, startTime);
  String output = Utilities::formatNumber(timeBetween / 3600, 2, 0) + ":";
  timeBetween %= 3600;
  output += Utilities::formatNumber(timeBetween / 60, 2, 0) + ":";
  timeBetween %= 60;
  output += Utilities::formatNumber(timeBetween, 2, 0);
  */
  return Utilities::timeBetween(gps.time, startTime);
}

// Date Module Implementation
String Date::date() {
  return Utilities::formatDate(gps.date);
}

String Date::timeofday() {
  return Utilities::formatTime(gps.time).substring(0, 5);
}

String Date::timeofdayseconds() {
  return Utilities::formatTime(gps.time);
}