
#ifndef TRAININGSCORER_H
#define TRAININGSCORER_H

#include "aircraft.h"
#include <vector>

class TrainingScorer {
public:
    static double calculateAverageSpacing(const std::vector<Aircraft>& aircraftList);
};

#endif // TRAININGSCORER_H
