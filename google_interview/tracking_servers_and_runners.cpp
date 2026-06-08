/**
Tracking Servers & Runners (OOD + state management)
This round mixes object design and coding.

You build a class that tracks runners under different servers.

Two required methods:
- getReport(serverId, runnerId, timestamp) 
    * Inputs arrive in chronological order. 
- getRunners(serverId) 
    * Return the active runners for a given server. 

Key skills they’re looking for:
- How you maintain internal state 
- Whether queries stay efficient 
- Whether your data structure choices make sense 

A clean approach often involves a doubly linked list combined with hash maps.
 */

#include <cstdint>
#include <list>
#include <vector>
#include <unordered_map>

using namespace std;

class RunnerTracker
{
private:
    struct RunnerNode
    {
        int runnerId;
        int64_t lastTimestamp;
    };

    struct ServerState
    {
        list<RunnerNode> active; // Doubly-linked list - O(1) to add and remove node at specific location

        unordered_map<
            int,
            list<RunnerNode>::iterator
        > lookup; // Locate runner in O(1)
    };

    unordered_map<int, ServerState> servers;
    int64_t timeout;

    void expire(ServerState& server, int64_t currentTime)
    {
        while (!server.active.empty())
        {
            auto& node = server.active.front();

            if (currentTime - node.lastTimestamp <= timeout)
                break;
            // O(1) - if it was a vector, to remove a node O(n)
            server.lookup.erase(node.runnerId);

            server.active.pop_front();
        }
    }

public:
    explicit RunnerTracker(
        long long timeoutWindow)
        : timeout(timeoutWindow)
    {}

    void getReport(int serverId,
                   int runnerId,
                   long long timestamp)
    {
        ServerState& server = servers[serverId];

        expire(server, timestamp);

        auto it = server.lookup.find(runnerId);
        // remove old active runner if exists
        if (it != server.lookup.end())
            server.active.erase(it->second);
        // update active runner node
        server.active.push_back(
        {
            runnerId,
            timestamp
        });
        // update lookup runner iterator
        auto nodeIt = prev(server.active.end());
        server.lookup[runnerId] = nodeIt;
    }

    vector<int> getRunners(
        int serverId,
        long long currentTime)
    {
        vector<int> result;
        auto serverIt = servers.find(serverId);

        if (serverIt == servers.end())
            return result;

        ServerState& server = serverIt->second;

        expire(server, currentTime);

        for (const auto& node : server.active)
            result.push_back(node.runnerId);

        return result;
    }
};
