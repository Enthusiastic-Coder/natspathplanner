
#include "AdvisoryManager.h"

std::string AdvisoryManager::getTurnAdvisory(Aircraft &aircraft, double targetHeading) {
    return aircraft.callsign + " TURN " + std::to_string((int)targetHeading) + " NOW";
}

std::string AdvisoryManager::getSpeedAdvisory(Aircraft &aircraft, double targetSpeed) {
    return aircraft.callsign + " REDUCE SPEED TO " + std::to_string((int)targetSpeed) + " KT";
}

std::string AdvisoryManager::getFinalInterceptAdvisory(Aircraft &aircraft) {
    return aircraft.callsign + " TURN FINAL " + std::to_string((int)(aircraft.heading - 30)) + "° INTERCEPT LOC";
}
