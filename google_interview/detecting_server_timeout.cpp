/**
Detecting Server Timeout
You’re given server logs, something like:
(serverId, timestamp, start/end)
Plus a timeout threshold.

Your goal: detect the earliest time a server times out.

The important part:
- You’re expected to process logs as they come in, not run a final pass at the end.
- It’s more about designing a correct, incremental state machine than writing a long solution.

The interviewer mainly evaluates:
- Your reasoning 
- How you structure data 
- Whether you handle edge cases cleanly
 */

#include <unordered_map>
#include <queue>
#include <vector>

using namespace std;

enum EventType
{
    START,
    END
};

struct LogEntry
{
    int serverId;
    int timestamp;
    EventType event;
};

struct TimeoutEntry
{
    int timeoutTime;
    int serverId;

    bool operator>(const TimeoutEntry& other) const
    {
        return timeoutTime > other.timeoutTime;
    }
};

class TimeoutDetector
{
private:
    int threshold;
    unordered_map<int, int> active;
    priority_queue<
        TimeoutEntry,
        vector<TimeoutEntry>,
        greater<TimeoutEntry>> pq;

public:
    TimeoutDetector(int timeout)
        : threshold(timeout)
    {}

    // Returns the earliest timeout detected
    // -1 means none yet.
    int process(const LogEntry& log)
    {
        while (!pq.empty() && pq.top().timeoutTime <= log.timestamp)
        {
            const auto top = pq.top();
            // O(1)
            pq.pop();

            auto it = active.find(top.serverId);
            if (it == active.end())
                continue; // stale entry - not active

            if (it->second + threshold == top.timeoutTime)
                return top.timeoutTime;
        }

        if (log.event == START)
        {
            active[log.serverId] = log.timestamp;
            // O(log N)
            pq.push({
                log.timestamp + threshold,
                log.serverId
            });
        }
        else
        {
            active.erase(log.serverId);
        }

        return -1;
    }
};
