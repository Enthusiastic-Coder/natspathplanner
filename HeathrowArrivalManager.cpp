#include "heathrowarrivalmanager.h"
#include <cmath>
#include <iostream>

HeathrowArrivalManager::HeathrowArrivalManager() {}

void HeathrowArrivalManager::manageArrivals(std::vector<Aircraft>& aircraftList, double runwayLat, double runwayLon) {
    applyWakeTurbulenceSpacing(aircraftList);

    for (auto& aircraft : aircraftList) {
        double distanceToRunway = calculateDistanceToRunway(aircraft, runwayLat, runwayLon);
        aircraft.distanceToRunway = distanceToRunway;

        if (distanceToRunway <= calculateBaseTurnDistance(aircraft)) {
            handleBaseTurn(aircraft, runwayLat, runwayLon);
        } else if (distanceToRunway <= 5.0) {
            handleFinalApproachIntercept(aircraft, runwayLat, runwayLon);
        }
    }
}

void HeathrowArrivalManager::assignInitialVectors(Aircraft& aircraft) {
    aircraft.altitude_ft = 10000;
    aircraft.speed = 220;

    if (aircraft.holdFix == "BNN" || aircraft.holdFix == "LAM") {
        controller.assignHeading(aircraft, 180);  // North downwind
    } else {
        controller.assignHeading(aircraft, 360);  // South downwind
    }

    manageRunwayAlternation(aircraft);

    std::cout << "Initial vectors assigned to " << aircraft.callsign << ", assigned runway " << aircraft.assignedRunway << std::endl;
}

void HeathrowArrivalManager::handleBaseTurn(Aircraft& aircraft, double runwayLat, double runwayLon) {
    double baseHeading = (aircraft.assignedRunway == "27L" || aircraft.assignedRunway == "27R") ? 270 : 90;

    controller.assignHeading(aircraft, baseHeading);
    controller.assignSpeed(aircraft, 180);
    controller.assignAltitude(aircraft, 4000);

    std::cout << "Base turn advisory for " << aircraft.callsign << " to heading " << baseHeading << std::endl;
}

void HeathrowArrivalManager::handleFinalApproachIntercept(Aircraft& aircraft, double runwayLat, double runwayLon) {
    double interceptHeading = calculateFinalInterceptHeading(aircraft, runwayLat, runwayLon);

    controller.assignHeading(aircraft, interceptHeading);
    controller.assignSpeed(aircraft, 160);
    controller.assignAltitude(aircraft, 3000);

    std::cout << "Final approach intercept advisory for " << aircraft.callsign << " to heading " << interceptHeading << std::endl;
}

void HeathrowArrivalManager::manageRunwayAlternation(Aircraft& aircraft) {
    aircraft.assignedRunway = currentRunway;
    aircraftSinceLastRunwaySwitch++;

    if (aircraftSinceLastRunwaySwitch >= 10) {
        currentRunway = (currentRunway == "27L") ? "27R" : "27L";
        aircraftSinceLastRunwaySwitch = 0;
        std::cout << "Runway alternation - now using " << currentRunway << std::endl;
    }
}

void HeathrowArrivalManager::applyWakeTurbulenceSpacing(std::vector<Aircraft>& aircraftList) {
    for (size_t i = 1; i < aircraftList.size(); ++i) {
        Aircraft& leader = aircraftList[i - 1];
        Aircraft& follower = aircraftList[i];

        double requiredSeparation = SpacingLogic::calculateRequiredSeparation(leader, follower);

        if (follower.distanceToRunway - leader.distanceToRunway < requiredSeparation) {
            controller.assignSpeed(follower, follower.speed - 10);
            std::cout << "Spacing advisory for " << follower.callsign << " to reduce speed for spacing with " << leader.callsign << std::endl;
        }
    }
}

double HeathrowArrivalManager::calculateBaseTurnDistance(const Aircraft& aircraft) const {
    if (aircraft.wakeCategory == WakeCategory::Heavy) return 10.0;
    if (aircraft.wakeCategory == WakeCategory::Medium) return 8.0;
    return 6.0;
}

double HeathrowArrivalManager::calculateFinalInterceptHeading(const Aircraft& aircraft, double runwayLat, double runwayLon) const {
    double deltaY = runwayLat - aircraft.latitude;
    double deltaX = runwayLon - aircraft.longitude;
    double heading = atan2(deltaX, deltaY) * 180.0 / M_PI;
    if (heading < 0) heading += 360.0;
    return heading;
}

double HeathrowArrivalManager::calculateDistanceToRunway(const Aircraft& aircraft, double runwayLat, double runwayLon) const {
    return SpacingLogic::calculateDistanceToRunway(aircraft, runwayLat, runwayLon);
}

std::string HeathrowArrivalManager::wakeCategoryToString(WakeCategory category) {
    switch (category) {
        case WakeCategory::Heavy: return "Heavy";
        case WakeCategory::Medium: return "Medium";
        case WakeCategory::Light: return "Light";
        default: return "Unknown";
    }
}
