
#include <cmath>
#include <iostream>
#include "simulate_approach.cpp"

void captureLocalizer(Aircraft& aircraft, double runwayLat, double runwayLon) {
    double deltaY = runwayLat - aircraft.latitude;
    double deltaX = runwayLon - aircraft.longitude;

    double desiredHeading = atan2(deltaX, deltaY) * 180.0 / M_PI;
    if (desiredHeading < 0) desiredHeading += 360.0;

    if (fabs(aircraft.heading_deg - desiredHeading) > 30) {
        aircraft.heading_deg += (desiredHeading > aircraft.heading_deg) ? 5 : -5;
    } else {
        aircraft.heading_deg = desiredHeading;
    }

    std::cout << aircraft.callsign << " CAPTURING LOCALIZER. Adjusting heading to " 
              << aircraft.heading_deg << "°" << std::endl;
}

void captureGlideslope(Aircraft& aircraft, double runwayLat, double runwayLon) {
    double distanceToRunway = aircraft.calculateDistanceNM(aircraft.latitude, aircraft.longitude, runwayLat, runwayLon);
    double requiredAltitude = distanceToRunway * 300;

    if (aircraft.altitude_ft > requiredAltitude + 100) {
        aircraft.verticalSpeed_fpm = -700;
    } else {
        aircraft.verticalSpeed_fpm = -500;
    }

    std::cout << aircraft.callsign << " CAPTURING GLIDESLOPE. Target Altitude: " 
              << requiredAltitude << " ft. VS: " << aircraft.verticalSpeed_fpm << " fpm" << std::endl;
}

void joinFinalApproach(Aircraft& aircraft, double runwayLat, double runwayLon, double runwayHeading) {
    double distanceToRunway = aircraft.calculateDistanceNM(aircraft.latitude, aircraft.longitude, runwayLat, runwayLon);
    captureLocalizer(aircraft, runwayLat, runwayLon);
    captureGlideslope(aircraft, runwayLat, runwayLon);

    if (distanceToRunway < 5.0) {
        aircraft.speed = std::max(140.0, aircraft.speed - 10.0);
    }

    std::cout << aircraft.callsign << " ON FINAL APPROACH. Speed: " 
              << aircraft.speed << " knots, Altitude: " << aircraft.altitude_ft 
              << " ft, Heading: " << aircraft.heading_deg << "°" << std::endl;
}
    