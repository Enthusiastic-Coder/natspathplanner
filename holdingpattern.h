#ifndef HOLDINGPATTERN_H
#define HOLDINGPATTERN_H

#include <string>

class HoldingPattern {
public:
    std::string fixName;
    double latitude;
    double longitude;
    int inboundHeading;   // Holding inbound leg heading (degrees)
    double legDistanceNM; // One leg length in NM

    HoldingPattern(const std::string& name, double lat, double lon, int inboundHdg, double legNM);
};

#endif
