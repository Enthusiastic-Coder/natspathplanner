
#include "Aircraft.h"

Aircraft::Aircraft(std::string callsign, double lat, double lon, double alt, double spd, double hdg) 
    : callsign(callsign), latitude(lat), longitude(lon), altitude(alt), speed(spd), heading(hdg) {}

void Aircraft::updatePosition(double dt) {
    // Simple position update based on heading and speed (assuming no wind)
    double distance = (speed / 3600.0) * dt; // Convert knots to nautical miles per second
    latitude += distance * cos(heading * M_PI / 180.0);
    longitude += distance * sin(heading * M_PI / 180.0);
}

void Aircraft::assignHeading(double newHeading) { heading = newHeading; }
void Aircraft::assignSpeed(double newSpeed) { speed = newSpeed; }
void Aircraft::assignAltitude(double newAltitude) { altitude = newAltitude; }
