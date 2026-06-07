/**
Solve this interview question using C or C++ language, whichever seems appropriate:
Question — Tree Islands Problem:
In a tree of zeros and ones, an island is defined as a group of ones that are surrounded by zeros or are at the boundary of the tree.

Example:

        0(1)
       /    \
   1(2)      1(3)
    /          \
  0(5)         1(4)
   /  \
 1(6)  1(7)
  \      \
 1(8)    1(9)
 
Find the number of islands in the tree.
In the above example, there are 4 islands.

Follow-up
Find the number of distinct island sizes.
Size = number of 1s in the island.

In the above example, the distinct sizes are 1 and 2.

Tree Islands
Given a binary tree of 0s and 1s, an island is a connected group of 1s separated by 0s or boundaries.

To solve:
- DFS to find connected components 
- Count the number of components 
- Follow-up: compute distinct component sizes
 */
#include <iostream>
#include <unordered_set>

struct TreeNode
{
    int value;      // 0 or 1

    TreeNode* left;
    TreeNode* right;

    TreeNode(int v)
        : value(v), left(nullptr), right(nullptr) {}
};

int islandSize(TreeNode* node)
{
    if (node == nullptr)
        return 0;

    if (node->value == 0)
        return 0;

    return 1
         + islandSize(node->left)
         + islandSize(node->right);
}

void dfs(TreeNode* node,
         TreeNode* parent,
         int& islandCount,
         std::unordered_set<int>& distinctSizes)
{
    if (node == nullptr)
        return;

    bool islandRoot =
        node->value == 1 &&
        (parent == nullptr || parent->value == 0);

    if (islandRoot)
    {
        ++islandCount;

        const int size = islandSize(node);

      distinctSizes.insert(size);
    }

    dfs(node->left,
        node,
        islandCount,
        distinctSizes);

    dfs(node->right,
        node,
        islandCount,
        distinctSizes);
}

struct Result
{
    int islandCount;
    int distinctSizeCount;
};

Result countIslands(TreeNode* root)
{
    int islands = 0;
    std::unordered_set<int> distinctSizes;

    dfs(root,
        nullptr,
        islands,
        distinctSizes);

    return {
        islands,
        (int)distinctSizes.size()
    };
}

// More efficient way
int dfsSize(
            TreeNode* node,
            TreeNode* parent,
            int& islands,
            std::unordered_set<int>& sizes)
{
    if (node == nullptr)
        return 0;

    int left =
        dfsSize(node->left,
                node,
                islands,
                sizes);

    int right =
        dfsSize(node->right,
                node,
                islands,
                sizes);

    if (node->value == 0)
        return 0;

    int currentSize =
        1 + left + right;

    bool islandRoot =
        (parent == nullptr ||
         parent->value == 0);

    if (islandRoot)
    {
        islands++;
        sizes.insert(currentSize);
    }

    return currentSize;
}
// Worst case: O(N); Balanced tree: O(log N)
Result countIslands(TreeNode* root)
{
    int islands = 0;
    std::unordered_set<int> sizes;

    dfsSize(root,
            nullptr,
            islands,
            sizes);

    return {
        islands,
        (int)sizes.size()
    };
}
