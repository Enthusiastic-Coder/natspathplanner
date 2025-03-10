
#include <cmath>
#include <iostream>
#include "simulate_approach.cpp"

void adjustForSpacing(std::vector<Aircraft>& aircraftList) {
    for (size_t i = 1; i < aircraftList.size(); ++i) {
        Aircraft& leader = aircraftList[i - 1];
        Aircraft& current = aircraftList[i];

        double actualSeparation = current.calculateDistanceNM(
            current.latitude, current.longitude, leader.latitude, leader.longitude
        );

        double requiredSeparation = 5.0; // Default wake turbulence spacing

        if (actualSeparation < requiredSeparation) {
            current.downwindExtensionNM += 2.0;
            current.speed = std::max(160.0, current.speed - 10.0);

            std::cout << current.callsign << " EXTENDING downwind by 2NM and reducing speed to " 
                      << current.speed << " knots for spacing." << std::endl;
        }
    }
}

void manageBaseTurns(std::vector<Aircraft>& aircraftList) {
    for (size_t i = 1; i < aircraftList.size(); ++i) {
        Aircraft& leader = aircraftList[i - 1];
        Aircraft& current = aircraftList[i];

        double actualSeparation = current.calculateDistanceNM(
            current.latitude, current.longitude, leader.latitude, leader.longitude
        );

        double requiredSeparation = 5.0;
        double baseTurnDelay = std::max(0.0, requiredSeparation - actualSeparation);

        if (baseTurnDelay > 0) {
            current.baseTurnExtensionNM += baseTurnDelay;
            std::cout << current.callsign << " DELAYING base turn by " << baseTurnDelay << " NM." << std::endl;
        }

        if (current.distanceToRunway <= 10.0 + current.baseTurnExtensionNM && current.altitude_ft <= 4000) {
            double baseHeading = (current.assignedRunway == "27L" || current.assignedRunway == "27R") ? 270 : 90;
            current.heading_deg = baseHeading;
            current.speed = 180;
            current.altitude_ft = 4000;

            std::cout << current.callsign << " BASE TURN initiated at adjusted distance." << std::endl;
        }
    }
}
    