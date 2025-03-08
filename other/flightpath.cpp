#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <cmath>

struct Segment {
    double lengthNM;
    std::string name;
};

class FlightPath {
public:
    void updateUpwind(double distanceNM) {
        if (!upwindLocked) {
            if (segments.empty() || segments.back().name != "Upwind") {
                segments.push_back({"Upwind", distanceNM});
            } else {
                segments.back().lengthNM += distanceNM;
            }
        }
    }

    void lockUpwindAndAddDownwind(double downwindNM, double baseNM, double finalNM) {
        if (!upwindLocked) {
            upwindLocked = true;
            segments.push_back({downwindNM, "Downwind"});
            segments.push_back({baseNM, "Base"});
            segments.push_back({finalNM, "Final"});
        }
    }

    double calculateTrackingDistance() const {
        double totalDistance = 0.0;
        for (const auto& segment : segments) {
            totalDistance += segment.lengthNM;
        }
        return totalDistance;
    }

    void printSegments() const {
        for (const auto& segment : segments) {
            std::cout << segment.name << ": " << segment.lengthNM << " NM\n";
        }
    }

private:
    std::vector<Segment> segments;
    bool upwindLocked = false;
};

double calculateGhostOffset(double groundSpeed, int timeRemainingSeconds, int timeBasedSeparationSeconds) {
    double groundSpeedPerSecond = groundSpeed / 60.0;
    double targetDistance = groundSpeedPerSecond * timeRemainingSeconds;
    double separationDistance = groundSpeedPerSecond * timeBasedSeparationSeconds;

    return targetDistance - separationDistance;
}

double getCurrentGroundSpeed(int timeRemainingSeconds) {
    if (timeRemainingSeconds > 600) {
        return 240.0;
    } else if (timeRemainingSeconds > 300) {
        return 210.0;
    } else {
        return 180.0;
    }
}

int main() {
    FlightPath flightPath;

    auto now = std::chrono::system_clock::now();
    auto targetArrivalTime = now + std::chrono::minutes(15);
    int timeBasedSeparationSeconds = 120;  

    bool turnedDownwind = false;

    while (true) {
        now = std::chrono::system_clock::now();
        auto durationToTarget = std::chrono::duration_cast<std::chrono::seconds>(targetArrivalTime - now);
        int timeRemainingSeconds = durationToTarget.count();

        if (timeRemainingSeconds <= 0) {
            std::cout << "Aircraft should be on final approach now.\n";
            flightPath.printSegments();
            break;
        }

        double groundSpeed = getCurrentGroundSpeed(timeRemainingSeconds);

        // Simulate 0.2 NM per second at 240 knots
        flightPath.updateUpwind(groundSpeed / 360.0);  

        if (!turnedDownwind && timeRemainingSeconds <= 900) {  
            std::cout << "TURNING DOWNWIND!\n";
            flightPath.lockUpwindAndAddDownwind(10.0, 3.0, 4.0);  
            turnedDownwind = true;
        }

        double trackingDistance = flightPath.calculateTrackingDistance();
        double ghostOffset = calculateGhostOffset(groundSpeed, timeRemainingSeconds, timeBasedSeparationSeconds);

        std::cout << "Time remaining: " << timeRemainingSeconds << " sec | GS: " << groundSpeed 
                  << " knots | Tracking Distance: " << trackingDistance 
                  << " NM | Ghost Offset: " << ghostOffset << " NM\n";

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}