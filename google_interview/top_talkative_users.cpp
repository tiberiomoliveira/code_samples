/**
Given a log file, find the top N most talkative users: 

10:00 <john> hi!
10:01 <maria> hello!
10:07 <john> can you link the design?

// Returns e.g.
// [
//   {username: "john", wordCount: 1},
//   {username: "maria", wordCount: 1},
//   {username: "john", wordCount: 5}
// ]
public List<UserWordCount> parseLog(String filepath) {** Implemented for you **} 

Parse each line, extract username and message, count words, aggregate per user in a map, sort by total word count (desc), return top N.

Given a chat log, count how many words each user has spoken and return the top N most talkative users.

Key steps:
- Parse each log line to extract the username. 
- Split the message to count the number of words. 
- Accumulate counts in a HashMap. 
- Sort users by total word count (descending) and return the top N.
 */

#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

using namespace std;

struct UserWordCount
{
    string username;
    int wordCount;
};

struct UserTotal
{
    string username;
    long long totalWords;
};

vector<UserTotal>
topNMostTalkative(const string& filepath, size_t N)
{
    vector<UserWordCount> records = parseLog(filepath);
    unordered_map<string, long long> totals;

    for (const auto& record : records)
    {
        totals[record.username] += record.wordCount;
    }

    vector<UserTotal> users;
    for (const auto& entry : totals)
    {
        users.push_back(
        {
            entry.first,
            entry.second
        });
    }

    sort(users.begin(),
         users.end(),
         [](const UserTotal& a,
            const UserTotal& b)
         {
             if (a.totalWords != b.totalWords)
                 return a.totalWords >
                        b.totalWords;

             return a.username <
                    b.username;
         });

    if (users.size() > N)
    {
        users.resize(N);
    }

    return users;
}
