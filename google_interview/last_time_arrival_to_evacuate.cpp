/**
# You're on a remote island that needs evacuation due to pending flooding.
# The government is sending planes tomorrow to get some people out.
# If you miss the last plane you'd have to take a slower boat the following day which you don't want.
# Given the plane schedule and the schedule of all your fellow islanders,
# What is the latest you could get to the airport tomorrow and still get evacuated?

This Google interview problem describes an evacuation scenario: multiple flights leave
the island tomorrow, and every islander has their own arrival time at the airport.
Assuming planes fill up in departure-time order and seats are taken by whoever arrives
earlier, you must compute the latest time you personally can arrive at the airport and
still guarantee a seat on some plane, instead of being forced onto the slow boat. It
tests your ability to model schedules, capacities, and ordering, often using sorting
and careful simulation or greedy reasoning.
 */

#include <iostream>
#include <vector>
#include <algorithm>

struct Plane {
    int time;
    int capacity;
};

class EvacuationPlanner {
private:
    // Simulates the boarding process to see if 'our_time' guarantees us a seat
    bool canEvacuate(const std::vector<Plane>& planes, 
                     const std::vector<int>& islanders, // Islanders' arrival time
                     int our_time) {
        int i = 0; // Pointer to the current islander in the queue

        for (const auto& plane : planes) {
            int seats = plane.capacity;
            
            while (seats > 0) {
                // Check who is physically at the airport before this plane leaves
                const bool islander_ready = (i < islanders.size() &&
                                             islanders[i] <= plane.time);
                const bool we_ready = our_time <= plane.time;

                if (islander_ready && we_ready) {
                    // TIE BREAKER: If an islander arrived before or at the exact 
                    // same time as us, assume the worst case: they get on first.
                    if (islanders[i] <= our_time) {
                        i++;
                        seats--;
                    } else {
                        // We arrived strictly before this islander!
                        return true; 
                    }
                } else if (islander_ready) {
                    i++;
                    seats--;
                } else if (we_ready) {
                    // We are at the airport, but the next islander isn't here yet
                    return true; 
                } else {
                    // Neither we nor the remaining islanders are at the airport 
                    // before this plane departs. The plane leaves with empty seats.
                    break;
                }
            }
        }
        
        // All planes have departed and we never boarded
        return false;
    }

public:
    // Returns the latest possible arrival time, or -1 if evacuation is impossible
    int getLatestArrivalTime(std::vector<Plane> planes, std::vector<int> islanders) {
        // 1. Sort planes by departure time
        std::sort(planes.begin(), planes.end(),
            [](const Plane& a, const Plane& b) {
                return a.time < b.time;
        });

        // 2. Sort islanders by arrival time
        std::sort(islanders.begin(), islanders.end());

        // 3. Binary Search our arrival time
        int low = 0;
        int high = planes.empty() ? 0 : planes.back().time;
        int latest_time = -1;

        while (low <= high) {
            int mid = low + (high - low) / 2;
            
            if (canEvacuate(planes, islanders, mid)) {
                latest_time = mid; // This time works, save it
                low = mid + 1;     // But try to arrive even later
            } else {
                high = mid - 1;    // We missed the flight, we must arrive earlier
            }
        }

        return latest_time;
    }
};

// --- Example Usage ---
int main() {
    EvacuationPlanner planner;
    
    std::vector<Plane> planes = {{10, 1}, {20, 1}}; // Time, Capacity
    std::vector<int> islanders = {5, 15};

    // Explanation: 
    // Plane at 10 takes the islander who arrived at 5.
    // Plane at 20 takes the islander who arrived at 15.
    // If we arrive at 14, we get in front of the islander arriving at 15, taking the 20 plane.
    std::cout << "Latest arrival time: " 
              << planner.getLatestArrivalTime(planes, islanders) << "\n"; // Output: 14

    return 0;
}
