/**
Let’s pretend we’re working on a computer vision project. In this project,
librarians sent us videos of their bookshelves. Based on frames from these
videos, we want to quickly and efficiently find the spine tags on the books.
For the purpose of the interview, we will be simplifying this to the following
problem:

Given an m x n binary matrix, find the largest square containing only 1s and return its area.

Example:

Input matrix:
0000000000000000
0000000000000000
0000000000000000
0000001111100000
0000001111100000
0000001111100000
0000001111100000
0000001111100000
0000000000000000
0000000000000000
0000000000000000
0000000000000000
0000000000000000

This is a classic dynamic programming problem on a binary matrix. For each cell,
compute the side length of the largest square of 1s that ends at that cell using
the minimum of its top, left, and top-left neighbors plus one. Track the maximum
side length across the matrix, then return its square as the area. It is a common
OA/interview question that tests how well you model a 2D DP state.
*/

// Almost same question as the 'find_a_spinter_tag_in_a_bookshelf.cpp'. Difference
// is that this one asks for the area.

uint32_t maximal_square(std::vector<std::vector<uint8_t>>& matrix) {
    // Edge cases
    if (matrix.empty() || matrix[0].empty()) {
        return 0;
    }
    const uint32_t rows = matrix.size();
    const uint32_t cols = matrix.size();
    uint32_t max_sqr_side = 0;
    // dp[r][c] represents the side length of the maximum square whose
    // bottom-right corner is at matrix[r - 1][c - 1].
    // We pad the DP table with an extra row and column of 0s to simplify boundary checks.
    std::vector<std::vector<int>> dp(rows + 1, std::vector<int>(cols + 1, 0));
    for (uint32_t r = 0; r <= rows; ++r) {
        for (uint32_t c = 0; c <= cols; ++c) {
            // We only care about cells that contain a '1' in the original matrix
            if (matrix[r][c] == 1) {
                // The largest square ending here is constrained by its top, left, and
                // top-left neighbors. We take the minimum of these three and add 1
                dp[r][c] = std::min({
                    dp[r - 1][c],    // top
                    dp[r][c - 1],    // left
                    dp[r - 1][c - 1] // top-left
                });
                // Update the maximum side of the square found so far
                max_sqr_side = std::max(max_sqr_side, dp[r][c]); 
            }
        }
    }
    // Return the area of the square
    return max_sqr_side * max_sqr_side;
}
