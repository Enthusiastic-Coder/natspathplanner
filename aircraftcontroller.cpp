#include "aircraftcontroller.h"
#include <iostream>
#include <cmath>

AircraftController::AircraftController() {}

void AircraftController::assignHeading(Aircraft& aircraft, double heading) {
    aircraft.heading_deg = heading;
    std::cout << aircraft.callsign << " Heading set to " << heading << " degrees." << std::endl;
}

void AircraftController::assignSpeed(Aircraft& aircraft, double speed) {
    aircraft.speed = speed;
    std::cout << aircraft.callsign << " Speed set to " << speed << " knots." << std::endl;
}

void AircraftController::assignAltitude(Aircraft& aircraft, double altitude) {
    aircraft.altitude_ft = altitude;
    std::cout << aircraft.callsign << " Altitude set to " << altitude << " feet." << std::endl;
}

void AircraftController::assignVerticalSpeed(Aircraft& aircraft, double vs) {
    aircraft.verticalSpeed_fpm = vs;
    std::cout << aircraft.callsign << " Vertical speed set to " << vs << " fpm." << std::endl;
}

void AircraftController::directToFix(Aircraft& aircraft, double fixLat, double fixLon) {
    double deltaY = fixLat - aircraft.latitude;
    double deltaX = fixLon - aircraft.longitude;
    double desiredHeading = atan2(deltaX, deltaY) * 180.0 / M_PI;
    if (desiredHeading < 0) desiredHeading += 360.0;

    assignHeading(aircraft, desiredHeading);
    std::cout << aircraft.callsign << " Direct to fix (" << fixLat << ", " << fixLon << "), heading " << desiredHeading << " degrees." << std::endl;
}
