#include <vector>
#include <cmath>
#include <algorithm>

const double MAX_LOCALIZER_DISTANCE = 20.0;  // Max distance for the localizer in miles
const double MAX_UPWIND_DISTANCE = 5.0;  // Max upwind distance in miles
const double MAX_DOWNWIND_DISTANCE = 10.0;  // Max downwind distance in miles
const double SEPARATION_DISTANCE = 3.0;  // Minimum horizontal separation distance between aircraft in the localizer in miles
const double MIN_SPEED_KTS = 120.0;  // Minimum speed (knots) for slower aircraft to maintain spacing
const double ALTITUDE_SEPARATION = 1000.0; // Minimum altitude separation (in feet)

struct Aircraft {
    double x, y;  // Position (in miles, relative to runway threshold)
    double altitude;  // Altitude (in feet)
    double speed;  // Speed (in knots)
    double course;  // Heading (degrees)
    bool isHolding; // Flag to determine if the aircraft is holding

    // Constructor
    Aircraft(double x_, double y_, double altitude_, double speed_, double course_)
        : x(x_), y(y_), altitude(altitude_), speed(speed_), course(course_), isHolding(false) {}

    // Calculate distance to the runway threshold (assumed to be at (0, 0) in this model)
    double distanceToRunway() const {
        return std::sqrt(x * x + y * y);
    }

    // Check if the aircraft is aligned with the localizer course (bearing check)
    bool isOnLocalizer(double localizerCourse) const {
        double angleDiff = std::abs(course - localizerCourse);
        return angleDiff < 10.0 || angleDiff > 350.0;  // Allow for small deviation (e.g., 10 degrees)
    }

    // Check if the aircraft is within the maximum upwind or downwind distances
    bool isWithinDistance(double maxDistance, bool isUpwind) const {
        if (isUpwind) {
            return y > 0 && y <= maxDistance;  // Upwind direction (aircraft is moving against the wind)
        } else {
            return y < 0 && std::abs(y) <= maxDistance;  // Downwind direction (aircraft is parallel to runway)
        }
    }

    // Ensure aircraft speed is adjusted to avoid overtaking others
    void adjustSpeed(double targetSpeed) {
        if (speed > targetSpeed) {
            speed = targetSpeed;
        }
    }

    // Check if the aircraft is vertically separated from another aircraft
    bool isVerticallySeparated(const Aircraft& otherAircraft) const {
        return std::abs(altitude - otherAircraft.altitude) >= ALTITUDE_SEPARATION;
    }
};

bool compareAircraftDistance(const Aircraft &a1, const Aircraft &a2) {
    return a1.distanceToRunway() < a2.distanceToRunway();  // Closer aircraft first
}

enum FlightSegment {
    Upwind,
    Downwind,
    Base,
    FinalApproach
};

// Function to determine the segment type based on the aircraft's position
FlightSegment determineSegment(const Aircraft& ac, double finalApproachX, double finalApproachY) {
    // Assuming the final approach line is aligned along the y-axis (runway centerline)
    if (ac.y > finalApproachY) {
        return Upwind; // Aircraft is moving into the wind, before turning downwind
    }
    else if (ac.y < finalApproachY) {
        return Downwind; // Aircraft is parallel to runway, opposite direction
    } 
    else if (ac.x < 0) {
        return Base; // Aircraft is turning to base leg to join final
    } else {
        return FinalApproach; // Aircraft is final and aligned with the runway
    }
}

// Function to determine the best turn direction and path for aircraft separation
void calculateApproachPath(std::vector<Aircraft>& aircrafts, double finalApproachX, double finalApproachY, double localizerCourse) {
    // Sort aircraft based on distance to runway to determine landing order
    std::sort(aircrafts.begin(), aircrafts.end(), compareAircraftDistance);

    // Vector to hold aircraft that are eligible for the localizer
    std::vector<Aircraft> localizerAircrafts;

    // Filter aircraft based on localizer distance, bearing, and max upwind/downwind distance
    for (auto& ac : aircrafts) {
        if (ac.distanceToRunway() <= MAX_LOCALIZER_DISTANCE && ac.isOnLocalizer(localizerCourse)) {
            // Check if the aircraft is within upwind or downwind limits
            if ((ac.isWithinDistance(MAX_UPWIND_DISTANCE, true) || ac.isWithinDistance(MAX_DOWNWIND_DISTANCE, false))) {
                localizerAircrafts.push_back(ac);
            }
        }
    }

    // Now, handle aircraft separation within the localizer
    for (size_t i = 0; i < localizerAircrafts.size(); ++i) {
        Aircraft& ac = localizerAircrafts[i];
        FlightSegment segment = determineSegment(ac, finalApproachX, finalApproachY);

        switch (segment) {
            case Upwind:
                // Aircraft needs to proceed upwind before making a downwind turn.
                break;
            case Downwind:
                // Aircraft is in the downwind leg, and needs to prepare to turn towards the base.
                if (i > 0) {
                    Aircraft& prevAc = localizerAircrafts[i - 1];
                    double distanceBetween = std::sqrt(std::pow(ac.x - prevAc.x, 2) + std::pow(ac.y - prevAc.y, 2));
                    if (distanceBetween < SEPARATION_DISTANCE && ac.isVerticallySeparated(prevAc)) {
                        // Aircraft has vertical separation but not enough horizontal distance
                        // Extend the downwind or delay the base leg
                        ac.adjustSpeed(MIN_SPEED_KTS);  // Slow down the aircraft to maintain separation
                        ac.isHolding = true;  // Mark as holding
                        std::cout << "Aircraft " << i << " is too close to aircraft " << i - 1 << " and needs to slow down." << std::endl;
                    }
                }
                break;
            case Base:
                // Aircraft is preparing for base leg turn. Adjust spacing and angle.
                break;
            case FinalApproach:
                // Aircraft is aligned on final approach. Ensure no conflict with aircraft in base or downwind.
                if (i > 0) {
                    Aircraft& prevAc = localizerAircrafts[i - 1];
                    double distanceBetween = std::sqrt(std::pow(ac.x - prevAc.x, 2) + std::pow(ac.y - prevAc.y, 2));
                    if (distanceBetween < SEPARATION_DISTANCE) {
                        // Adjust the aircraft speed or hold the aircraft if too close
                        ac.adjustSpeed(MIN_SPEED_KTS);  // Slow down the aircraft to maintain separation
                        std::cout << "Aircraft " << i << " is too close to aircraft " << i - 1 << " and needs to slow down." << std::endl;
                    }
                }
                break;
        }
    }
}

int main() {
    // Sample aircraft data (x, y, altitude, speed, course)
    std::vector<Aircraft> aircrafts = {
        Aircraft(2.5, 5.0, 2500, 150, 90), // aircraft 1
        Aircraft(3.0, 7.0, 3000, 160, 90), // aircraft 2
        Aircraft(5.0, 3.5, 1500, 140, 270), // aircraft 3 (off course, will be ignored)
        Aircraft(1.0, 6.0, 2500, 155, 90) // aircraft 4
    };

    // Coordinates of the final approach point (center of the runway)
    double finalApproachX = 0.0;
    double finalApproachY = 5.0;

    // Localizer course direction (typically 90 degrees for an east-facing runway)
    double localizerCourse = 90.0;

    // Calculate the best approach paths for the aircraft
    calculateApproachPath(aircrafts, finalApproachX, finalApproachY, localizerCourse);

    return 0;
}
