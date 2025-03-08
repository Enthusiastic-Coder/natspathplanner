
#include "SpacingLogic.h"

double SpacingLogic::calculateRequiredSeparation(const Aircraft &leader, const Aircraft &follower) {
    if (leader.wakeCategory == "Heavy" && follower.wakeCategory != "Heavy") return 6.0; // 6NM separation
    return 4.0; // Default spacing
}
