#include "spacinglogic.h"
#include <cmath>
#include <algorithm>
#include <iostream>

double SpacingLogic::calculateDistanceToRunway(const Aircraft& aircraft, double runwayLat, double runwayLon) {
    return aircraft.calculateDistanceNM(aircraft.latitude, aircraft.longitude, runwayLat, runwayLon);
}

double SpacingLogic::calculateRequiredSeparation(const Aircraft& lead, const Aircraft& trail) {
    if (lead.wakeCategory == WakeCategory::Heavy && trail.wakeCategory == WakeCategory::Medium) return 5.0;
    if (lead.wakeCategory == WakeCategory::Heavy && trail.wakeCategory == WakeCategory::Light) return 6.0;
    return 3.0;  // Standard IFR separation for same wake category.
}

std::vector<Aircraft> SpacingLogic::sortAircraftByDistanceToRunway(const std::vector<Aircraft>& aircraftList, 
                                                                   double runwayLat, double runwayLon) {
    std::vector<Aircraft> sortedList = aircraftList;

    std::sort(sortedList.begin(), sortedList.end(), 
              [runwayLat, runwayLon](const Aircraft& a, const Aircraft& b) {
        double distA = calculateDistanceToRunway(a, runwayLat, runwayLon);
        double distB = calculateDistanceToRunway(b, runwayLat, runwayLon);
        return distA < distB;
    });

    return sortedList;
}

void SpacingLogic::generateAdvisories(std::vector<Aircraft>& aircraftList, 
                                      double runwayLat, double runwayLon, 
                                      AircraftController& controller) {
    auto sortedList = sortAircraftByDistanceToRunway(aircraftList, runwayLat, runwayLon);

    for (size_t i = 0; i < sortedList.size(); ++i) {
        Aircraft& current = sortedList[i];
        double distanceToRunway = calculateDistanceToRunway(current, runwayLat, runwayLon);
        current.distanceToRunway = distanceToRunway;

        if (i > 0) {
            Aircraft& leader = sortedList[i - 1];
            double requiredSeparation = calculateRequiredSeparation(leader, current);
            double actualSeparation = current.distanceToRunway - leader.distanceToRunway;

            if (actualSeparation < requiredSeparation) {
                controller.assignSpeed(current, current.speed - 10);  // Slow down to rebuild separation
                std::cout << current.callsign << " advisory: reduce speed to " 
                          << current.speed << " knots to increase spacing behind " 
                          << leader.callsign << std::endl;
            }
        }

        // Basic advisory generation (descend, final speed, etc.)
        if (distanceToRunway < 10.0 && current.altitude_ft > 5000) {
            controller.assignAltitude(current, 4000);
            std::cout << current.callsign << " advisory: descend to 4000 ft" << std::endl;
        } else if (distanceToRunway < 5.0 && current.altitude_ft > 3000) {
            controller.assignAltitude(current, 3000);
            controller.assignSpeed(current, 160);
            std::cout << current.callsign << " advisory: descend to 3000 ft and slow to 160 knots" << std::endl;
        }
    }
}
