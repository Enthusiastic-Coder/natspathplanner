
#include "trainingscorer.h"
#include <algorithm>

double TrainingScorer::calculateAverageSpacing(const std::vector<Aircraft>& aircraftList) {
    if (aircraftList.size() < 2) return 0;

    double totalSpacing = 0;
    for (size_t i = 0; i < aircraftList.size() - 1; ++i) {
        totalSpacing += aircraftList[i].calculateDistanceNM(
            aircraftList[i].latitude, aircraftList[i].longitude,
            aircraftList[i+1].latitude, aircraftList[i+1].longitude
        );
    }
    return totalSpacing / (aircraftList.size() - 1);
}
