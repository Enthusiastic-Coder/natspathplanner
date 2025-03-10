
#include <iostream>
#include <vector>
#include <cmath>

const double RUNWAY_27L_LAT = 51.4775;
const double RUNWAY_27L_LON = -0.4614;

struct Aircraft {
    std::string callsign;
    double latitude, longitude, altitude_ft, speed, heading_deg, verticalSpeed_fpm;
    bool isTurning = false;
    double downwindExtensionNM = 0.0;
    double baseTurnExtensionNM = 0.0;

    Aircraft(std::string cs, double lat, double lon, double alt, double spd, double hdg)
        : callsign(cs), latitude(lat), longitude(lon), altitude_ft(alt), speed(spd), heading_deg(hdg) {}

    double calculateDistanceNM(double lat1, double lon1, double lat2, double lon2) {
        const double R_E = 3440.0;  
        double dLat = (lat2 - lat1) * M_PI / 180.0;
        double dLon = (lon2 - lon1) * M_PI / 180.0;
        double a = sin(dLat / 2) * sin(dLat / 2) +
                   cos(lat1 * M_PI / 180.0) * cos(lat2 * M_PI / 180.0) *
                   sin(dLon / 2) * sin(dLon / 2);
        double c = 2 * atan2(sqrt(a), sqrt(1 - a));
        return R_E * c;
    }
};

void updateAircraftWithWind(Aircraft& aircraft, double windSpeed, double windDirection, double timeStep);
void updateTurnWithWind(Aircraft& aircraft, double windSpeed, double windDirection, double timeStep);
void captureLocalizer(Aircraft& aircraft, double runwayLat, double runwayLon);
void captureGlideslope(Aircraft& aircraft, double runwayLat, double runwayLon);
void joinFinalApproach(Aircraft& aircraft, double runwayLat, double runwayLon, double runwayHeading);
void adjustForSpacing(std::vector<Aircraft>& aircraftList);
void manageBaseTurns(std::vector<Aircraft>& aircraftList);

void runSimulation(std::vector<Aircraft>& aircraftList, double windSpeed, double windDirection, double totalTime) {
    double timeStep = 5.0;
    for (double t = 0; t < totalTime; t += timeStep) {
        std::cout << "\n--- Simulation Time: " << t << " sec ---\n";
        for (size_t i = 0; i < aircraftList.size(); ++i) {
            Aircraft& current = aircraftList[i];
            if (current.calculateDistanceNM(current.latitude, current.longitude, RUNWAY_27L_LAT, RUNWAY_27L_LON) < 16.0) {
                joinFinalApproach(current, RUNWAY_27L_LAT, RUNWAY_27L_LON, 270.0);
            } else {
                updateAircraftWithWind(current, windSpeed, windDirection, timeStep);
            }
        }
        adjustForSpacing(aircraftList);
        manageBaseTurns(aircraftList);
    }
}
    