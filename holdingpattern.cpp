
#include "holdingpattern.h"

HoldingPattern::HoldingPattern(const std::string& name, double lat, double lon, int heading, double legNM)
    : fixName(name), fixLatitude(lat), fixLongitude(lon), inboundHeading(heading), legDistanceNM(legNM) {}
