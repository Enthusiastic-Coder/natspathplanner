#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <string>

enum class WakeCategory { Heavy, Medium, Light };

class Aircraft {
public:
    std::string callsign;
    double latitude;
    double longitude;
    double altitude_ft;
    double speed;
    double heading_deg;
    double verticalSpeed_fpm;
    std::string holdFix;  // Assigned holding fix (BNN, OCK, etc.)
    std::string assignedRunway;
    double distanceToRunway;  // Updated dynamically
    WakeCategory wakeCategory;

    Aircraft(const std::string& callsign, double lat, double lon, double alt, double spd, double hdg, double vs, WakeCategory wake);

    void updatePosition(double timeStepSeconds);
    double calculateDistanceNM(double lat1, double lon1, double lat2, double lon2) const;
    bool isInConflictWith(const Aircraft& other, double lateralNM, double verticalFt) const;
    void reportStatus() const;
};

#endif
