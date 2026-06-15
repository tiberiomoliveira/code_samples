/**
Given the head of a singly linked list, reverse the list, and return the reversed list.

Additional information
The number of nodes in the list is in the range [0, 5000].
-5000 <= Node.val <= 5000

Example 1:
Input: head = [1, 2, 3, 4, 5]
Output: [5, 4, 3, 2, 1]

Example 2:
Input: head = [1, 2]
Output: [2, 1]

Example 3:
Input: head = []
Output: []

NOTE: If there's a loop, the Floyd's cycle detection algorithm can be used to detect it
      before reverting the linked list.
*/

typedef struct Node {
    uint8_t value;
    Node* next;
};

Node* reverse(Node* head) {
    Node* prev = nullptr;
    Node* curr = head;
    // In-place reverse
    while (curr) {
        // First, store `next` 
        Node* next = curr->next;
        // Update the next
        curr->next = prev;
        // Update pointers
        prev = curr;
        curr = next;
    }

    return prev;
}
