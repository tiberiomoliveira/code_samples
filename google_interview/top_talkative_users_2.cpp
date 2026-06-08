/**
Imagine a log file that abridges chats. An example chat might be:
10:00 <john> hi!
10:01 <maria> hello!
10:07 <john> can you link the design?

where john said 6 total words and maria said 1.

Your goal is to present the top N most talkative users in descending order.

You need to write a function that takes an integer N and a filepath.
You are provided a helper function, parse_log, which returns a list of
usernames and word counts for each message parsed from the provided file
[(‘john’, 1), (‘maria’, 1), (‘john’, 5)] in the example above.

Output: [“john”, “maria”]

In this Google interview problem, you are given a chat log where each line
has a timestamp, a username, and a message. A helper function parse_log has
already broken the log into tuples of (username, word_count) for each message.
Your task is to aggregate the counts per user, rank users by how many words
they spoke in total, and return the top N usernames in descending order of
word count, resolving ties by order implied in the data if needed.
 */

#include <algorithm>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

struct MessageInfo
{
    string username;
    int wordCount;
};

// assuming this function is implemented
vector<MessageInfo>
parse_log(const string& filepath);

vector<string>
topNMostTalkative(int N,
                  const string& filepath)
{
    unordered_map<string, int64_t> totals;
    vector<pair<string, int64_t>> users;
    vector<MessageInfo> messages = parse_log(filepath);
    // use a HashMap to aggregate word count
    for (const auto& msg : messages)
        totals[msg.username] += msg.wordCount;
    // transform map into vector to sort by word count
    for (const auto& entry : totals)
        users.push_back(entry);

    sort(users.begin(),
         users.end(),
         [](const auto& a,
            const auto& b)
         {
             // try to sort by the word count
             if (a.second != b.second)
                 return a.second > b.second;
             // if word count is equal, then sort by name
             return a.first < b.first;
         });

    vector<string> result;
    const int limit = min(N, static_cast<int>(users.size()));

    for (int i = 0; i < limit; i++)
        result.push_back(users[i].first);

    return result;
}
