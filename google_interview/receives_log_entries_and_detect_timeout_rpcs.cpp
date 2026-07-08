/**
Imagine you have an RPC server that produces log entries and you're
analyzing it offline. There are two entries for each call: one when
the RPC starts and one when the RPC finishes processing.

Your task is to efficiently determine if any RPC in the log timed out.

You can assume your function receives two arguments:
- A timeout duration.
- A collection of pre-parsed log entries, already sorted by timestamp.

To test your understanding, consider the following example:

Input timeout: 3
Input log:
| ID | Timestamp | Event Type |
|----|-----------|------------|
| 1  | 0         | Start      |
| 2  | 1         | Start      |
| 1  | 2         | End        |
| 3  | 6         | Start      |
| 2  | 7         | End        |
| 3  | 8         | End        |

Graphical representation of the log:
ID 1: [0, 2]
ID 2: [1, 7]
ID 3: [6, 8]

This problem asks you to inspect a time-ordered RPC log and determine
whether any request exceeded a given timeout. Since each RPC has a
Start and End entry, the standard approach is a single pass with a
hash map that stores the start timestamp for each active ID. When the
End entry appears, compute the elapsed time and compare it with the
timeout. The solution is efficient, with linear time complexity and
small auxiliary space.
 */

enum class EventType {
    start,
    end
};
struct LogEntry {
    int id;
    int timestamp;
    EventType type;
};

bool has_rpc_timeout(int timeout_duration, const std::vector<LogEntry>& logs) {
    // Hash map to store the start time of active RPCs
    // key: RPC ID, value: start timestamp
    std::unordered_map<int, int> active_rpcs;
    for (const auto& log : logs) {
        if (log.type == EventType::Start) {
            // Save the start time for this RPC
            active_rpcs[log.id] = log.timestamp;
        } else if (log.type == EventType::End) {
            // Find the corresponding Start entry
            auto it = active_rpcs.find(log.id);
            if (it != active_rpcs.end()) {
                const int start_time = it->second;
                const int duration = log.timestamp - start_time;
                // Check if the duration exceeds the allowed timeout
                if (duration > timeout_duration) {
                    // Timeout detected
                    return true;
                }
                // Clean up the map to save mem
                active_rpcs.erase(it);
            }
        }
    }
    return false;
}

// ==========================================
// Test Driver
// ==========================================
int main() {
    int timeout = 3;
    
    // Constructing the log based on the provided example
    vector<LogEntry> logs = {
        {1, 0, EventType::Start},
        {2, 1, EventType::Start},
        {1, 2, EventType::End},
        {3, 6, EventType::Start},
        {2, 7, EventType::End},
        {3, 8, EventType::End}
    };

    bool result = hasRpcTimedOut(timeout, logs);
    
    if (result) {
        cout << "Result: True (At least one RPC timed out).\n";
    } else {
        cout << "Result: False (No RPCs timed out).\n";
    }

    return 0;
}
