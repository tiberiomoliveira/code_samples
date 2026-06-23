/**
Write a node class to be used in a tree. A Node should contain exactly one integer and have zero or more children.

Define: a "descending tree" is a tree such that for each node n, the value of n is larger than the value of any descendant of n.

        1000
      /   |   \
   500   200   700
   / \     |     / \
300 100   ...   50  150
 
Write a function that takes a node, which is the root of a tree, and determines if it is the root of a descending tree.

Write a function that takes a node, which is the root of a descending tree, and returns the largest leaf value.

This Google interview set focuses on defining a flexible tree node class (each node stores one integer and has zero
or more children), then introduces a special structure called a "descending tree". The property is simple but
strong: every node must be strictly larger than all of its descendants. Based on this definition, the first task
is to implement a function to verify whether a given root satisfies this property, requiring a recursive check
across all children. The second task is to return the largest leaf value in a valid descending tree, which boils
down to exploring all leaf nodes and selecting the maximum.
 */

#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

class Node {
public:
    int value;
    std::vector<Node*> children;

    // Constructor for easy node creation
    Node(int val) : value(val) {}
};

bool isDescendingTree(const Node* root) {
    // An empty tree/node is valid
    if (!root) return true;

    for (const Node* child : root->children) {
        if (!child) continue;

        // 1. Immediate check: Parent must be strictly greater than the child
        if (root->value <= child->value) {
            return false;
        }

        // 2. Recursive check: The child's subtree must also be a descending tree
        if (!isDescendingTree(child)) {
            return false;
        }
    }
    return true;
}

void findLargestLeafHelper(const Node* root, int& max_leaf) {
    if (!root) return;

    // PRUNING OPTIMIZATION:
    // If this node's value is less than or equal to the max leaf found so far,
    // none of its descendants can possibly beat our current maximum.
    if (root->value <= max_leaf) {
        return;
    }

    // Base Case: If it's a leaf node (no children), update our maximum
    if (root->children.empty()) {
        max_leaf = std::max(max_leaf, root->value);
        return;
    }

    // Recursive Case: Traverse children
    for (const Node* child : root->children) {
        findLargestLeafHelper(child, max_leaf);
    }
}

int getLargestLeafValue(const Node* root) {
    if (!root) return INT_MIN; // Return a sentinel value for an empty tree
    
    int max_leaf = INT_MIN;
    findLargestLeafHelper(root, max_leaf);
    return max_leaf;
}
