#ifndef AIRCRAFTCONTROLLER_H
#define AIRCRAFTCONTROLLER_H

#include "aircraft.h"

class AircraftController {
public:
    AircraftController();

    void assignHeading(Aircraft& aircraft, double heading);
    void assignSpeed(Aircraft& aircraft, double speed);
    void assignAltitude(Aircraft& aircraft, double altitude);
    void assignVerticalSpeed(Aircraft& aircraft, double vs);
    void directToFix(Aircraft& aircraft, double fixLat, double fixLon);
};

#endif
