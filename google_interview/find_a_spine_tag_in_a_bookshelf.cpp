/**
...
Let's pretend we're working on a computer vision project. In this project,
librarians sent us videos of their bookshelves. Based on frames from these videos,
we want to quickly and efficiently find the spine tags on the library books.
For the purposes of the interview, we'll be simplifying this to the following problem.

input matrix - m x n = m ~ n = all 0s  
spine tag - square of 1s

000000000000
000000000000
000000000000
000000000000
000001111000
000001111000
000001111000
000001111000
000000000000
000000000000

This Google interview problem simplifies a computer vision task: given a frame represented
as an m×n matrix of 0s and 1s, where a spine tag corresponds to a square submatrix of 1s,
determine how to detect that square quickly and efficiently. The challenge typically lies
in finding the top-left coordinate and size of the square of 1s, ideally in linear or
near-linear time, rather than checking every possible square with brute force.
 */


struct SpineTag {
    int8_t row;
    int8_t col;
    uint32_t size;
};

class LibraryScanner {
    public:
        // Finds the top-left coordinate and size of the square of 1s
        SpineTag find_spine_tag(const std::vector<std::vector<int8_t>>& img) {
            // Edge cases
            if (img.empty() || img[0].empty()) {
                return {-1, -1, 0};
            }
            const uint32_t row = img.size();
            const uint32_t col = img[0].size();
            // Scan the image row by row
            for (uint32_t r = 0; r < row; ++r) {
                for (uint32_t c = 0; c < col; ++c) {
                    // The first '1' we find is strictly the top-left corner
                    if (img[r][c] == 1) {
                        uint32_t size = 0;
                        // Measure the width of the square/spine-tag
                        while (c + size < col && img[r][c + size] == 1) {
                            ++size;
                        }
                        // Return the top-left coordinate and the size
                        return {r, c, size};
                    }
                }
            }
            // Couldn't find a spine-tag
            return {-1, -1, 0};
        }
};

// --- Testing the Implementation ---
int main() {
    std::vector<std::vector<int>> image = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    LibraryScanner scanner;
    SpineTag tag = scanner.findSpineTag(image);

    std::cout << "Top-Left Corner: (" << tag.row << ", " << tag.col << ")\n";
    std::cout << "Square Size: " << tag.size << "x" << tag.size << "\n";

    return 0;
}
