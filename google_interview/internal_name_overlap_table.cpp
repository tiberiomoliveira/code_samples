/**
You are given a table of people with three columns: Name, Start, and End.
Each row represents an interval for a person.

Create a second table with columns Start, End, and Names. The output
table should break the timeline into consecutive intervals using every
unique start and end value from the input. For each interval, list all
names whose original intervals cover that entire segment.

Example:
Name   | Start | End
Abby   | 10    | 100
Ben    | 50    | 70
Carla  | 60    | 120
David  | 150   | 300


Output:
Start | End | Names
10    | 50  | Abby
50    | 60  | Abby, Ben
60    | 70  | Abby, Ben, Carla
70    | 100 | Abby, Carla
100   | 120 | Carla
150   | 300 | David

This problem is about splitting the timeline into atomic intervals and
finding which original ranges cover each segment. A practical solution
is to collect all unique start and end points, sort them, and then examine
each adjacent pair as one output interval. For each segment, include
every name whose interval fully covers that range. The example shows how
overlapping intervals produce merged name lists across consecutive segments.
 */

// Input struct
struct Person {
    std::string name;
    uint32_t start;
    uint32_t end;
};
// Output struct
struct IntervalOutput {
    uint32_t start;
    uint32_t end;
    std::vector<std::string> names;
};

class IntervalSplitter {
public:
    std::vector<IntervalOutput> split_time(const std::vector<Person>& people) {
        // Collect all unique start and end points using a set
        std::set<uint32_t> unique_times;
        for (const auto& person : people) {
            unique_times.insert(person.start);
            unique_times.insert(person.end);
        }
        // Convert the set to a vector for easier index-based access
        std::vector<uin32_t> times(unique_times.begin(), unique_times.end());
        std::vector<InternalOutput> result;
        // Create consecutive intervals from adjacent unique points
        for (size_t i = 0; i < times.size(); ++i) {
            const uint32_t timeline_start = times[i];
            const uint32_t timeline_end = times[i + 1];
            std::vector<std::string> active_names;
            // Find all people whose original interval covers this timeline
            for (const auto& person : persons) {
                if (person.start <= timeline_start &&
                    person.end >= timeline_end) {
                    active_names.push_back(person.name);
                }
            }
            // If at least one person covers this timeline, add it to our results
            if (!active_names.empty()) {
                result.push_back({
                    timeline_start,
                    timeline_end,
                    active_names
                });
            }
        }
        return result;
    }
};

// Helper function to print results and test the example
int main() {
    std::vector<Person> people = {
        {"Abby", 10, 100},
        {"Ben", 50, 70},
        {"Carla", 60, 120},
        {"David", 150, 300}
    };

    IntervalSplitter splitter;
    std::vector<IntervalOutput> result = splitter.splitTimeline(people);

    std::cout << "Start\t| End\t| Names\n";
    std::cout << "---------------------------\n";
    for (const auto& res : result) {
        std::cout << res.start << "\t| " << res.end << "\t| ";
        for (size_t i = 0; i < res.names.size(); ++i) {
            std::cout << res.names[i] << (i < res.names.size() - 1 ? ", " : "");
        }
        std::cout << "\n";
    }

    return 0;
}
