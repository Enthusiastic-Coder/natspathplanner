
#ifndef HOLDINGPATTERNMANAGER_H
#define HOLDINGPATTERNMANAGER_H

#include "holdingpattern.h"
#include <vector>

class HoldingPatternManager {
public:
    static std::vector<HoldingPattern> loadHeathrowHoldings();
};

#endif // HOLDINGPATTERNMANAGER_H
