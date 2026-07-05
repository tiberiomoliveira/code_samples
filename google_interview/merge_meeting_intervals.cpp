/**
Merge Meeting Intervals with DNS Time Ranges

Can we assume the input is sorted or not?
You can sort by the start time of each interval.

Merge the overlapping meetings with a loop over the meetings. Then,
for each merged interval, remove the part that overlaps with the DNS
time ranges.

How would you merge the second and third meetings [12, 30] and [22, 35]?
12 starts before 22, so we need to check if they overlap.
The end of the first is 30 > 22, so we need to merge them.
The new interval becomes [12, 35].

Meetings: [(1, 7, 1), (5, 10, 2), (12, 30, 1), (22, 30, 1), (40, 50, 1), (60, 70, 1)]
DNS: [(18, 25), (20, 28), (65, 75)]
# Only merge the meetings

This problem combines interval merging with interval subtraction. First, sort the
meetings by start time and scan them linearly to merge any overlapping intervals.
Then, for each merged meeting interval, remove the portions that overlap with the
DNS ranges, splitting the interval when necessary. The key implementation details
are handling boundary overlap correctly and applying a clean sweep over sorted
intervals.
 */

#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

// Define our interval structure
struct Interval {
    int start;
    int end;
    int id = 0; // Optional: included to match the 3-tuple input format, though merging destroys unique IDs.
};

class MeetingScheduler {
public:
    std::vector<Interval> get_available_times(
            std::vector<Interval> meetings,
            std::vector<Interval> dns) {
        // 1. Sort and merge the original meetings
        std::vector<Interval> merged_meetings = merge_intervals(meetings);
        // 2. Sort and merge the DNS intervals 
        // (This prevents headaches from overlapping DNS blocks during subtraction)
        std::vector<Interval> merged_dns = merge_intervals(dns);
        std::vector<Interval> final_schedule;
        // 3. Subtract DNS blocks from the merged meetings
        for (const auto& meeting : merged_meetings) {
            // We use a queue to handle intervals that get split into multiple pieces
            std::queue<Interval> q;
            q.push(meeting);
            for (const auto& dns_block : merged_dns) {
                int current_pieces = q.size();
                for (int i = 0; i < current_pieces; ++i) {
                    Interval curr = q.front();
                    q.pop();
                    // Condition A: No overlap at all
                    if (curr.end <= dns_block.start ||
                        curr.start >= dns_block.end) {
                        q.push(curr);
                    } 
                    // Condition B: Overlap exists, potentially splitting the interval
                    else {
                        // Left leftover piece
                        if (curr.start < dns_block.start) {
                            q.push({curr.start, dns_block.start});
                        }
                        // Right leftover piece
                        if (curr.end > dns_block.end) {
                            q.push({dns_block.end, curr.end});
                        }
                    }
                }
            }
            // Any pieces of this meeting that survived all DNS blocks are valid
            while (!q.empty()) {
                final_schedule.push_back(q.front());
                q.pop();
            }
        }
        return final_schedule;
    }

private:
    // Helper function to merge a list of intervals
    std::vector<Interval> merge_intervals(std::vector<Interval>& intervals) {
        if (intervals.empty()) return {};
        // Sort intervals by their start time
        std::sort(intervals.begin(), intervals.end(),
            [](const Interval& a, const Interval& b) {
                return a.start < b.start;
        });
        std::vector<Interval> merged;
        merged.push_back(intervals[0]);
        for (size_t i = 1; i < intervals.size(); ++i) {
            if (intervals[i].start <= merged.back().end) {
                // Intervals overlap, merge them by extending the end time
                merged.back().end = std::max(merged.back().end, intervals[i].end);
            } else {
                // No overlap, push as a new distinct interval
                merged.push_back(intervals[i]);
            }
        }
        return merged;
    }
};

// Demonstration using your provided example
int main() {
    std::vector<Interval> meetings = {
        {1, 7, 1}, {5, 10, 2}, {12, 30, 1}, {22, 30, 1}, {40, 50, 1}, {60, 70, 1}
    };
    
    std::vector<Interval> dns = {
        {18, 25}, {20, 28}, {65, 75}
    };
    
    MeetingScheduler scheduler;
    std::vector<Interval> result = scheduler.get_available_times(meetings, dns);
    
    std::cout << "Final Available Meeting Times:\n";
    for (const auto& interval : result) {
        std::cout << "[" << interval.start << ", " << interval.end << "] ";
    }
    std::cout << "\n";
    
    return 0;
}
