
#include <cmath>
#include <iostream>
#include "simulate_approach.cpp"

void updateAircraftWithWind(Aircraft& aircraft, double windSpeed, double windDirection, double timeStep) {
    const double R_E = 3440.0;
    const double KNOTS_TO_NMPS = 1.0 / 3600.0;
    double V = aircraft.speed * KNOTS_TO_NMPS;
    double V_w = windSpeed * KNOTS_TO_NMPS;
    double theta = aircraft.heading_deg * M_PI / 180.0;
    double theta_w = windDirection * M_PI / 180.0;

    double V_g = sqrt(V * V + V_w * V_w - 2 * V * V_w * cos(theta - theta_w));
    double theta_c = theta - asin((V_w * sin(theta_w - theta)) / V);

    aircraft.latitude += (V_g * cos(theta_c) / R_E) * timeStep;
    aircraft.longitude += (V_g * sin(theta_c) / (R_E * cos(aircraft.latitude * M_PI / 180.0))) * timeStep;
    aircraft.heading_deg = theta_c * 180.0 / M_PI;
}

void updateTurnWithWind(Aircraft& aircraft, double windSpeed, double windDirection, double timeStep) {
    const double g = 9.81;
    const double KNOTS_TO_MPS = 0.51444;
    double speedMPS = aircraft.speed * KNOTS_TO_MPS;
    double bankAngleRad = 25.0 * M_PI / 180.0;
    double turnRadius = (speedMPS * speedMPS) / (g * tan(bankAngleRad));

    double windCorrectionFactor = (windSpeed / aircraft.speed) * turnRadius;
    aircraft.baseTurnExtensionNM += windCorrectionFactor / 60.0;

    double turnRate = speedMPS / (turnRadius + windCorrectionFactor);
    aircraft.heading_deg += (turnRate * timeStep) * (180.0 / M_PI);
    if (aircraft.heading_deg > 360.0) aircraft.heading_deg -= 360.0;
}
    