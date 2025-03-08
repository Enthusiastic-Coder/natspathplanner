#include <iostream>
#include <thread>
#include <chrono>
#include "aircraft.h"
#include "aircraftcontroller.h"
#include "heathrowarrivalmanager.h"
#include "scenariomanager.h"
#include "spacinglogic.h"
#include "holdingpatternmanager.h"

// Static position of Runway 27L threshold for spacing calculations (approx for Heathrow 27L)
const double RUNWAY_27L_LAT = 51.4775;
const double RUNWAY_27L_LON = -0.4614;

void printAircraftStatus(const std::vector<Aircraft>& aircraftList) {
    for (const auto& aircraft : aircraftList) {
        aircraft.reportStatus();
    }
    std::cout << "-----------------------------\n";
}

int main() {
    std::cout << "Heathrow Arrival Manager Simulation\n";

    // Load holdings and scenario
    auto holdings = HoldingPatternManager::loadHeathrowHoldings();
    auto aircraftList = ScenarioManager::loadDefaultScenario();

    AircraftController controller;
    HeathrowArrivalManager arrivalManager;

    // Assign initial vectors (simulates release from holds)
    for (auto& ac : aircraftList) {
        arrivalManager.assignInitialVectors(ac);
    }

    // Main simulation loop (simulating position updates, vectoring, spacing advisories)
    const int simulationSteps = 30;  // Run for 30 "ticks"
    for (int step = 0; step < simulationSteps; ++step) {
        std::cout << "\n--- Simulation Step " << step + 1 << " ---\n";

        // Move aircraft based on heading, speed, vertical speed
        for (auto& ac : aircraftList) {
            ac.updatePosition(30);  // Each step = 30 seconds
        }

        // Run arrival manager (spacing, vectoring, base turns, final intercept)
        arrivalManager.manageArrivals(aircraftList, RUNWAY_27L_LAT, RUNWAY_27L_LON);

        // Generate advisories (spacing, speed control, etc.)
        SpacingLogic::generateAdvisories(aircraftList, RUNWAY_27L_LAT, RUNWAY_27L_LON, controller);

        // Print aircraft positions and states
        printAircraftStatus(aircraftList);

        // Simulate time passage
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << "Simulation complete.\n";
    return 0;
}
