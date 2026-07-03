/**
"""
Implement a restaurant waitlist data structure. It should support the following features:

A party of customers can join the waitlist.
A previously joined party can leave the waitlist at any time.

The restaurant can ask the data structure for the first party that fits a given table
size (a table size is given as an argument).
"""

This Google data-structure problem requires designing a dynamic waitlist that supports
three operations efficiently: adding a party, removing a party at arbitrary time, and
querying the earliest (in arrival order) party whose size is less than or equal to a
given table capacity. A correct design often maintains ordering for fairness, but also
needs a fast way to find the first group that fits, which typically leads to combining
a queue-like structure with an auxiliary index—such as a hash map for O(1) removals and
a size-bucketed list or balanced tree for efficient“find first that fits.”
 */
#include <iostream>
#include <list>
#include <optional>
#include <string>
#include <unordered_map>

struct Party {
    std::string name;
    uint8_t size;
    uint64_t position;
};

class Waitlist {
    private:
        uint64_t current_pos = 0;
        // Maps a table/party size to a list of parties - std::list guarantees O(1) removals
        std::unordered_map<uint8_t, std::list<Party>> tables;
        // Maps a party name to its node in the list of parties for O(1) access
        std::unordered_map<std::string, std::list<Party>::iterator> directory;
    public:
        // Add party to waitlist
        void add_party(const std::string& name, uint8_t size) {
            // Check if party is already on the waitlist
            if (directory.find(name) != directory.end()) {
                return;
            }
            ++current_pos; // Party will be added to the waitlist
            Party new_party = {name, size, current_pos};
            // Add new party to the waitlist of the table defined by the size
            tables[size].push_back(new_party);
            // Save the iterator of the party for fast access
            directory[name] = std::prev(tables[size].end());
        }

        // Remove party that is leaving the waitlist early
        bool leave_waitlist(const std::string& name) {
            auto party_node = directory.find(name);
            // Check if party was found
            if (party_node == directory.end()) {
                return false;
            }
            const uint8_t party_size = party_node->second->size;
            auto party_iterator = party_node->second;
            // Remove party from table waitlist
            tables[party_size].erase(party_iterator);
            // Remove party from the directory
            directory.erase(party_node);

            return true;
        }

        // Find and remove the first party that fits the given table size
        std::optional<std::string> get_table(uint8_t table_size) {
            uint64_t earliest_pos = -1;
            uint8_t earliest_party_size = -1;
            // Check the front of the queue for all sizes up to the table capacity.
            for (uint8_t s = 1; s <= table_size; ++s) {
                auto table_waitlist = tables.find(s);
                // Find table waitlist for the given table size and check if there's a waitlist
                if (table_waitlist != tables.end() &&
                    !table_waitlist->second.empty()) {
                    // Get the party from the front of the waitlist
                    const Party& front_party = table_waitlist->second.front();
                    // Check the earliest arrival time/position
                    if (earliest_pos == -1 ||
                        front_party.position < earliest_ticket) {
                        earliest_party_size = s;
                    }
                }
            }
            // Check if a party was found for the given table
            if (earliest_party_size != -1) {
                const std::string party_name = tables[earliest_party_size].front().name;
                // Remove the party from the waitlist
                leave_waitlist(party_name);
                // Call for the party
                return party_name;
            }
            return std::nullopt; // No party fits the table
        }
};
