#include "scenariomanager.h"

std::vector<Aircraft> ScenarioManager::loadDefaultScenario() {
    return {
        Aircraft("BAW123", 51.723, -0.635, 12000, 250, 145, 0, WakeCategory::Heavy),   // BNN Hold
        Aircraft("EZY456", 51.566,  0.110, 11000, 240, 270, 0, WakeCategory::Medium),  // LAM Hold
        Aircraft("BAW789", 51.308, -0.443, 13000, 230,  87, 0, WakeCategory::Heavy),   // OCK Hold
        Aircraft("RYR321", 51.330,  0.030, 10000, 220, 110, 0, WakeCategory::Light),   // BIG Hold
        Aircraft("DLH101", 51.350, -0.200, 9000, 250, 270, 0, WakeCategory::Heavy)     // Random inbound aircraft
    };
}
