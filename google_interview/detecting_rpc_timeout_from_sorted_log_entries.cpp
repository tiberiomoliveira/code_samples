/**
Imagine you have an RPC server that produces log entries, and we’re analyzing it offline.
There are two entries for each call:
- one when the RPC starts,
- and one when the RPC finishes processing.


Your task is to efficiently determine if any RPC in the log timed out.

You can assume your function receives two arguments:
- A timeout duration (integer).
- A collection of pre-parsed log entries, already sorted by timestamp.


Example

Input timeout:
3

Input log:
ID Timestamp Event Type
1	0	Start
2	1	Start
2	6	End
3	6	Start
1	7	End
3	8	End

Graphical representation of the log

ID: 1     (------------)
ID: 2          (-----------)
ID: 3                 (------)
Time: 0 1 2 3 4 5 6 7 8
Timeout = 3

Check whether any RPC ran longer than 3 units.

Iterate through the log entries.

Maintain a hash map start[id] = timestamp.

When you see a Start, store its timestamp.

When you see an End, compute: duration = end - start[id]

If duration > timeout, immediately return - True (a timeout occurred).

If no entry exceeds timeout, return False.

Logs are already sorted → linear scan is optimal.

Time complexity: O(n)

Space complexity: O(n) (in worst case)
 */

#include <unordered_map>
#include <vector>

enum EventType
{
    START,
    END
};

struct LogEntry
{
    int id;
    int timestamp;
    EventType type;
};

bool hasTimedOut(
    int timeout,
    const std::vector<LogEntry>& logs)
{
    // Space: O(k)
    std::unordered_map<int, int> startTime;

    // O(n)
    for (const auto& entry : logs)
    {
        if (entry.type == START)
        {
            startTime[entry.id] =
                entry.timestamp;
        }
        else
        {
            auto it =
                startTime.find(entry.id);

            // is a malformed log
            if (it == startTime.end())
                continue;

            const int duration =
                entry.timestamp -
                it->second;

            if (duration > timeout)
                return true;

            startTime.erase(it);
        }
    }

    return false;
}
