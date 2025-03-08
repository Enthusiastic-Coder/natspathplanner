#ifndef HEATHROWARRIVALMANAGER_H
#define HEATHROWARRIVALMANAGER_H

#include "aircraft.h"
#include "aircraftcontroller.h"
#include "spacinglogic.h"
#include <vector>
#include <string>
#include <map>

class HeathrowArrivalManager {
public:
    HeathrowArrivalManager();

    void manageArrivals(std::vector<Aircraft>& aircraftList, double runwayLat, double runwayLon);
    void assignInitialVectors(Aircraft& aircraft);

private:
    AircraftController controller;
    std::string currentRunway = "27L";
    int aircraftSinceLastRunwaySwitch = 0;

    void handleBaseTurn(Aircraft& aircraft, double runwayLat, double runwayLon);
    void handleFinalApproachIntercept(Aircraft& aircraft, double runwayLat, double runwayLon);
    void manageRunwayAlternation(Aircraft& aircraft);
    void applyWakeTurbulenceSpacing(std::vector<Aircraft>& aircraftList);

    double calculateBaseTurnDistance(const Aircraft& aircraft) const;
    double calculateFinalInterceptHeading(const Aircraft& aircraft, double runwayLat, double runwayLon) const;
    double calculateDistanceToRunway(const Aircraft& aircraft, double runwayLat, double runwayLon) const;

    static std::string wakeCategoryToString(WakeCategory category);
};

#endif
