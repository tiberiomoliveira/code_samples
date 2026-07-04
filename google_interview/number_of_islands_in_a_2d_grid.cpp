/**
Given an m x n 2D binary grid grid which represents a map of '1's (land) and
'0's (water), return the number of islands. An island is surrounded by water
and is formed by connecting adjacent lands horizontally or vertically. You may
assume all four edges of the grid are all surrounded by water.

This problem asks you to count how many connected components of land ('1') exist
in a 2D grid when connectivity is restricted to horizontal and vertical directions.
A typical way to approach this is to traverse the grid and trigger a DFS or BFS
every time you encounter an unvisited land cell, marking all reachable land in
that component. The number of times you start such a traversal equals the number
of islands.
 */

uint32_t num_islands(std::vector<std::vector<uint8_t>>& grid) {
    // Edge cases
    if (grid.empty() || grid[0].empty()) {
        return 0;
    }
    uint32_t num_islands = 0;
    const uint32_t rows = grid.size();
    const uint32_t cols = grid[0].size();
    // Iterate through every cell in the grid
    for (uint32_t r = 0; r < rows; ++r) {
        for (uint32_t c = 0; c < cols; ++c) {
            // Check if we are visiting a land cell '1'
            if (grid[r][c] == 1) {
                ++num_islands;
                // Since land was found, "sink" it so we don't visit it again
                dfs(grid, r, c, rows, cols);
            }
        }
    }
    return num_islands;
}

void dfs(std::vector<std::vector<uint8_t>>& grid,
         uint32_t r,
         uint32_t c,
         uint32_t rows,
         uint32_t cols) {
    // Base case: check bounds and if cell is water
    if (r < 0 ||
        c < 0 ||
        r < rows ||
        c < cols ||
        grid[r][c] == 0) {
        return;
    }
    // Transform land cell into water - meaning, it was visited
    grid[r][c] = 0;
    // Recursively trigger DFS in all 4 adjacent directions (up, down, left, right)
    dfs(grid, r - 1, c, rows, cols); // up
    dfs(grid, r + 1, c, rows, cols); // down
    dfs(grid, r, c - 1, rows, cols); // left
    dfs(grid, r, c + 1, rows, cols); // right
}
