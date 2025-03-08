#ifndef SPACINGLOGIC_H
#define SPACINGLOGIC_H

#include "aircraft.h"
#include "aircraftcontroller.h"
#include <vector>

class SpacingLogic {
public:
    static double calculateDistanceToRunway(const Aircraft& aircraft, double runwayLat, double runwayLon);
    static double calculateRequiredSeparation(const Aircraft& lead, const Aircraft& trail);

    static std::vector<Aircraft> sortAircraftByDistanceToRunway(const std::vector<Aircraft>& aircraftList, double runwayLat, double runwayLon);

    static void generateAdvisories(std::vector<Aircraft>& aircraftList, 
                                   double runwayLat, double runwayLon, 
                                   AircraftController& controller);
};

#endif // SPACINGLOGIC_H
