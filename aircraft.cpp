#include "aircraft.h"
#include <cmath>
#include <iostream>

Aircraft::Aircraft(const std::string& callsign, double lat, double lon, double alt, double spd, double hdg, double vs, WakeCategory wake)
    : callsign(callsign), latitude(lat), longitude(lon), altitude_ft(alt), speed(spd), heading_deg(hdg), verticalSpeed_fpm(vs), wakeCategory(wake) {}

void Aircraft::updatePosition(double timeStepSeconds) {
    const double KNOTS_TO_FPS = 1.68781;
    double speed_fps = speed * KNOTS_TO_FPS;
    double headingRad = heading_deg * M_PI / 180.0;

    latitude += (speed_fps * timeStepSeconds / 6076.0) * cos(headingRad);
    longitude += (speed_fps * timeStepSeconds / (6076.0 * cos(latitude * M_PI / 180.0))) * sin(headingRad);
    altitude_ft += (verticalSpeed_fpm / 60.0) * timeStepSeconds;
}

double Aircraft::calculateDistanceNM(double lat1, double lon1, double lat2, double lon2) const {
    const double EARTH_RADIUS_NM = 3440.0;
    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;

    double a = sin(dLat / 2) * sin(dLat / 2) +
               cos(lat1 * M_PI / 180.0) * cos(lat2 * M_PI / 180.0) *
               sin(dLon / 2) * sin(dLon / 2);

    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return EARTH_RADIUS_NM * c;
}

bool Aircraft::isInConflictWith(const Aircraft& other, double lateralNM, double verticalFt) const {
    double distance = calculateDistanceNM(latitude, longitude, other.latitude, other.longitude);
    return (distance < lateralNM) && (std::abs(altitude_ft - other.altitude_ft) < verticalFt);
}

void Aircraft::reportStatus() const {
    std::cout << callsign << " Position (" << latitude << ", " << longitude << ") Alt: " << altitude_ft
              << " ft Spd: " << speed << " kts Hdg: " << heading_deg << " deg Assigned RWY: " << assignedRunway << std::endl;
}
