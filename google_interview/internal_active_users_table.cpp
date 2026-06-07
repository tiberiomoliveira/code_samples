/**

 */

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

struct UserInterval
{
    string name;
    int start;
    int end;
};

struct Event
{
    int time;
    bool isStart;
    string name;
};

struct OutputInterval
{
    int start;
    int end;
    vector<string> users;
};

vector<OutputInterval>
buildTimeline(const vector<UserInterval>& users)
{
    vector<Event> events;

    // O(N)
    for (const auto& u : users)
    {
        events.push_back({u.start, true, u.name});
        events.push_back({u.end, false, u.name});
    }

    // O(N log N)
    sort(events.begin(),
         events.end(),
         [](const Event& a,
            const Event& b)
         {
             if (a.time != b.time)
                 return a.time < b.time;
             /* END before START */
             return a.isStart < b.isStart;
         });

    vector<OutputInterval> result;
    set<string> active;
    int i = 0;
    int prevTime = events[0].time;

    // O(N log N)
    while (i < (int)events.size())
    {
        const int currentTime = events[i].time;

        if (!active.empty() && prevTime < currentTime)
        {
            OutputInterval interval;

            interval.start = prevTime;
            interval.end = currentTime;
            interval.users.assign(
                active.begin(),
                active.end()
            );

            result.push_back(interval);
        }

        // O(log N)
        while (i < (int)events.size() && events[i].time == currentTime)
        {
            if (events[i].isStart)
                active.insert(events[i].name);
            else
                active.erase(events[i].name);
            ++i;
        }

        prevTime = currentTime;
    }

    return result;
}
