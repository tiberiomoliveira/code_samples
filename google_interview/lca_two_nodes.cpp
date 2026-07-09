/**
Given pointers to two ImageTransformers, return a pointer to their Least Common Ancestor (LCA). The device in question is very memory constrained so this must be done with O(1) memory and O(logn) time.
// ImageTransformer type definition
struct ImageTransformer {
    ImageTransformer* parent;
    std::vector<ImageTransformer*> children;
};
//Clarifying Questions:
    - Are the two nodes guaranteed to be part of the same tree?
    - Can either of the input pointers be nullptr/NULL?
    - Is the tree strictly balanced?
 */

#include <iostream>

struct ImageTransformer {
    ImageTransformer* parent;
    std::vector<ImageTransformer*> children;
    // Constructor for easy testing
    ImageTransformer(ImageTransformer* p = nullptr) : parent(p) {}
};

// Helper function to calculate the node depth
int get_depth(ImageTransformer* node) {
    int depth = 0;
    while (node != nullptr) {
        ++depth;
        node = node->parent;
    }
    return depth;
}

ImageTransformer* get_LCA(ImageTransformer* p, ImageTransformer* q) {
    // Edge cases
    if (p == nullptr || q == nullptr) return nullptr;
    // Get the depth to the root for each node
    const int depth_p = get_depth(p);
    const int depth_q = get_depth(q);
    // Align both pointers to the same depth
    while (depth_p > depth_q) {
        p = p->parent;
        --depth_p;
    }
    while (depth_q > depth_p) {
        q = q->parent;
        --depth_q;
    }
    // Walk up together until the pointers intersect
    while (p != q) {
        p = p->parent;
        q = q->parent;
    }
    // If p and q are equal, we found the LCA
    // If they are nullptr, they were in different trees (disconnected)
    return p;
}

// ==========================================
// Test Driver
// ==========================================
int main() {
    // Constructing a balanced tree
    //        root
    //       /    \
    //      A      B
    //     / \      \
    //    C   D      E
    
    ImageTransformer root;
    ImageTransformer A(&root);
    ImageTransformer B(&root);
    ImageTransformer C(&A);
    ImageTransformer D(&A);
    ImageTransformer E(&B);

    ImageTransformer* lca = getLCA(&C, &D);
    if (lca == &A) std::cout << "Test 1 Passed: LCA of C and D is A\n";

    lca = getLCA(&C, &E);
    if (lca == &root) std::cout << "Test 2 Passed: LCA of C and E is root\n";

    lca = getLCA(&A, &root);
    if (lca == &root) std::cout << "Test 3 Passed: LCA of A and root is root\n";

    return 0;
}
