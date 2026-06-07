/**
You have a list of meetings in your calendar with a start and end time. You are very busy, so meetings can overlap.

You also have one “Do Not Schedule”(DNS) interval during which you don’t attend any meeting.

Any meeting schedule that overlaps with a DNS slot is automatically cut so that it does not overlap with the DNS slot anymore.

Return a list of non-overlapping time intervals when you are in a meeting.

Example diagram:
Meeting | ---- ------------- ---
         | ---- --------     --
DNS:         XXXX
---------------------------------------> t
RES:        | ----- XXXX ----- ---  --

Sample input (for simplicity, all intervals include the left point and exclude the right point):
Meetings: [(1, 7), (5, 10), (12, 30), (22, 30), (40, 50), (60, 70)]
DNS: (18, 25)

Sample output:
[(1, 10), (12, 18), (25, 30), (40, 50), (60, 70)]

Part 2
Now you have more than one DNS slot.
If DNS slots overlap, they are merged into one DNS slot; everything else works the same.
Return one list of non-overlapping time intervals when you’re busy with meetings,
and another list for the merged DNS intervals.

Sample input:
Meetings: [(1, 7), (5, 10), (12, 30), (22, 30), (40, 50), (60, 70)]
DNS: [(18, 25), (20, 28), (65, 75)]

Sample output:
Meetings: [(1, 10), (12, 18), (28, 30), (40, 50), (60, 65)]
DNS: [(18, 28), (65, 75)]

- You’re given meeting intervals and one or more DNS intervals.
- Any meeting time that overlaps a DNS interval must be trimmed to remove the overlapping part.
- If multiple DNS intervals exist, merge them first.
- Output two merged lists:
    - non-overlapping meeting intervals after trimming
    - merged DNS intervals

Key idea:
Merge → subtract DNS from each meeting → merge results.
 */

#include <vector>
#include <algorithm>

using namespace std;

struct Interval
{
    int start;
    int end;
};

vector<Interval> mergeIntervals(vector<Interval> intervals)
{
    if (intervals.empty())
        return {};

    sort(intervals.begin(),
         intervals.end(),
         [](const Interval& a,
            const Interval& b)
         {
             return a.start < b.start;
         });

    vector<Interval> result;

    result.push_back(intervals[0]);

    for (size_t i = 1; i < intervals.size(); ++i)
    {
        Interval& last = result.back();

        if (intervals[i].start <= last.end)
        {
            last.end =
                max(last.end,
                    intervals[i].end);
        }
        else
        {
            result.push_back(intervals[i]);
        }
    }

    return result;
}

vector<Interval>
subtractDNS(const vector<Interval>& meetings,
            const vector<Interval>& dns)
{
    vector<Interval> result;

    int j = 0;

    for (const auto& meeting : meetings)
    {
        int currentStart = meeting.start;

        while (j < dns.size() &&
               dns[j].end <= meeting.start)
        {
            ++j;
        }

        int k = j;

        while (k < dns.size() &&
               dns[k].start < meeting.end)
        {
            if (dns[k].start > currentStart)
            {
                result.push_back(
                {
                    currentStart,
                    dns[k].start
                });
            }

            currentStart =
                max(currentStart,
                    dns[k].end);

            if (currentStart >= meeting.end)
                break;

            ++k;
        }

        if (currentStart < meeting.end)
        {
            result.push_back(
            {
                currentStart,
                meeting.end
            });
        }
    }

    return result;
}

struct Output
{
    vector<Interval> meetings;
    vector<Interval> dns;
};

Output processSchedule(
    vector<Interval> meetings,
    vector<Interval> dns)
{
    meetings = mergeIntervals(meetings);
    dns = mergeIntervals(dns);

    vector<Interval> finalMeetings =
        subtractDNS(meetings, dns);

    return {finalMeetings, dns};
}
