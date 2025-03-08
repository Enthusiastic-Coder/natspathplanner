#ifndef SCENARIOMANAGER_H
#define SCENARIOMANAGER_H

#include "aircraft.h"
#include "holdingpatternmanager.h"
#include <vector>

class ScenarioManager {
public:
    static std::vector<Aircraft> loadDefaultScenario();
};

#endif
