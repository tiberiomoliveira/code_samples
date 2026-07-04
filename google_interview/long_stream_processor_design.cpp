/**
You're given a log stream of a chat application. Every log entry has the following fields:
- timestamp: long – the number of seconds that have elapsed since 1970-01-01.
- sender: string – username of the sender
- receiver: string – username of the receiver
- message_text: string – the message payload

We want to implement a log stream processor which supports two methods:
- RegisterEvent(timestamp, sender_username, receiver_username, message_text)
  - registers the event that the message has been sent
- GetMostActiveUser()
  - Returns the username with the largest number of conversations.
  - We count any amount of messages exchanged between two unique users as a single conversation.

A conversation is defined as a unique unordered pair of users.
Messages between A and B count once, regardless of direction or frequency.

You must:
- Track unique user pairs
- Maintain per-user conversation counts
- Efficiently return the max user

This problem tests whether you can design a streaming data structure that tracks
unique user pairs as single conversations and efficiently return the user with
the most conversations in real time.
 */

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

class ChatLogProcessor {
private:
    // Maps a user to a set of unique users they have conversed with.
    std::unordered_map<std::string, std::unordered_set<std::string>> conversations;
    // Tracking the running maximum for O(1) retrieval
    std::string most_active_user = "";
    int max_conversations = 0;

public:
    // Registers the event that a message has been sent
    void registerEvent(long timestamp, 
                       const std::string& sender, 
                       const std::string& receiver, 
                       const std::string& message_text) {
        // A conversation with oneself typically doesn't count as a unique pair
        if (sender == receiver) return;
        // Insert receiver into sender's set, and vice versa
        conversations[sender].insert(receiver);
        conversations[receiver].insert(sender);
        // Check if the sender is now the most active user
        const int sender_count = conversations[sender].size();
        if (sender_count > max_conversations) {
            max_conversations = sender_count;
            most_active_user = sender;
        }
        // Check if the receiver is now the most active user
        const int receiver_count = conversations[receiver].size();
        if (receiver_count > max_conversations) {
            max_conversations = receiver_count;
            most_active_user = receiver;
        }
    }

    // Returns the username with the largest number of conversations
    std::string getMostActiveUser() const {
        return most_active_user;
    }
};

// --- Testing the Implementation ---
int main() {
    ChatLogProcessor processor;

    processor.registerEvent(1620000000, "Alice", "Bob", "Hi Bob!");
    processor.registerEvent(1620000005, "Bob", "Alice", "Hey Alice!");
    processor.registerEvent(1620000010, "Alice", "Charlie", "Hi Charlie!");
    processor.registerEvent(1620000015, "Dave", "Alice", "Alice, you there?");

    std::cout << "Most active: " << processor.getMostActiveUser() << "\n"; 
    // Output: Alice (3 conversations: Bob, Charlie, Dave)

    processor.registerEvent(1620000020, "Bob", "Charlie", "Did you see Alice?");
    processor.registerEvent(1620000025, "Bob", "Dave", "Hey Dave");
    processor.registerEvent(1620000030, "Bob", "Eve", "Hi Eve");

    std::cout << "Most active: " << processor.getMostActiveUser() << "\n"; 
    // Output: Bob (4 conversations: Alice, Charlie, Dave, Eve)

    return 0;
}
