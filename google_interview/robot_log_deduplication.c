/**
Warmup Section
Imagine you have a robot that sends status messages that humans will read in real time.
The raw messages are hard to read for a human because there are often many messages produced in short periods of time. One idea to make them more readable is to remove the duplicate messages over a 10-second window.

Design and implement a program to hide duplicates of any message that has already been displayed within the past 10 seconds.

Example Messages Received, with Timestamps:
10 solar panel activated
11 low battery warning
12 tire one: low air pressure
13 solar panel activated
14 low battery warning
21 solar panel activated
35 solar panel activated

Example Messages Shown to User:
10 solar panel activated
11 low battery warning
12 tire one: low air pressure
21 solar panel activated
35 solar panel activated

Updated Requirement Section
From experience operating the robot, users have determined there is a bug in the robot and duplicate messages are not to be trusted at all. Duplicate messages should be completely removed from the output if they occur within 10 seconds.

Design and implement a program to completely remove duplicate messages.

This problem asks you to process a stream of timestamped robot messages and apply a 10-second deduplication rule:

Part 1
If a message has already been shown within the past 10 seconds, hide the new occurrence.

Part 2 (Stricter)
If a message occurs more than once within any 10-second window, the message type is considered faulty — remove all occurrences of that message.

This requires:
- A sliding 10-second window per message 
- Tracking message timestamps 
- Detecting duplicates and suppressing output accordingly
 */

// Maybe better implement this on cpp
typedef struct MessageNode
{
    int timestamp;
    int invalid;
    char text[256];

    struct MessageNode *next_global;
    struct MessageNode *next_same_msg;
} MessageNode;

void processMessage(int timestamp,
                    const char *text)
{
    flushExpired(timestamp);

    // Better use a hash table to store the messages.
    MessageQueue *q =
        lookupMessage(text);

    while (!empty(q) &&
           timestamp - front(q)->timestamp > 10)
    {
        pop_front(q);
    }

    if (!empty(q))
    {
        MessageNode *p = front(q);

        while (p)
        {
            p->invalid = 1;
            p = p->next_same_msg;
        }
    }

    MessageNode *node =
        createNode(timestamp, text);

    if (!empty(q))
        node->invalid = 1;

    append(q, node);
    appendGlobal(node);
}

// At the end of the stream
void handleEndOfTheStream() {
    flushExpired(INFINITY);
}
