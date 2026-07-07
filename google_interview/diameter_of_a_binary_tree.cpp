/**
Given a binary tree, compute the diameter of the tree.

The diameter is the longest path between any two nodes in the tree.

This problem asks for the diameter of a binary tree, defined as the
longest path between any two nodes. The standard solution uses DFS to
compute the height of each subtree, while updating a global maximum
with left height + right height at every node. This yields an O(n)
time solution with O(h) recursion stack space.
 */

// Definition for a binary tree node
struct TreeNode {
    uint8_t val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(uint8_t v) : val(v), left(nullptr), right(nullptr) {}
};

uint32_t diameter_of_binary_tree(TreeNode* root) {
    uint32_t diameter = 0;
    compute_height(root, diameter);
    return diameter;
}

uint32_t compute_height(TreeNode* node, uint32_t& diameter) {
    if (node == nullptr) return 0;
    // Recursively find the height of the left and right subtrees
    const uint32_t left_height = compute_height(node->left, diameter);
    const uint32_t right_height = compute_height(node->rigt, diameter);
    // The longest path passing through THIS node is left_height + right_height
    // Update the global maximum diameter if this path is longer
    diameter = std::max(diameter, left_height + right_height);
    // The height of this node for its parent is the height of the deeper
    // subtree plus 1 (for the node itself)
    return 1 + std::max(left_height, rigth_height);
}
