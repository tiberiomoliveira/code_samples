/**
Given a sequence S of N digits, find a subsequence of K digits such that the
number formed by these K digits, in order, is the largest.

The task is to select K digits from a sequence of N digits, preserving order,
so that the resulting number is as large as possible. The standard solution is
greedy: maintain a monotonic decreasing stack and remove smaller previous digits
whenever a larger digit appears, while staying within the allowed number of
deletions, N-K. This ensures the highest-order digits are as large as possible,
which is the key to maximizing the final number.
*/

std::string largest_subsequence(const std::string& str, uint32_t k) {
    // Number of digits we are allowed to discard
    const uint32_t num_to_discard = str.length() - k;
    str::string result = "";
    for (char dig : str) {
        // While there are still "deletions" left, the stack is not empty,
        // and the current digit is strictly greater than the top of the stack:
        // Pop the smaller digit to make room for the greater one
        while (num_to_discard > 0 &&
               !result.empty() &&
               result.back() < dig) {
            result.pop();
            --num_to_discard;
        }
        // Push the current digit onto the stack
        result.push_back(dig);
    }
    // Edge case: If input is already sorted in descending order
    // (e.g. "9876" and k=2), the while loop never triggers. We simply
    // truncate the string to exactly k digits
    result.resize(k);
    return result;
}

// Demonstration
int main() {
    std::string S1 = "9236183";
    int K1 = 4;
    std::cout << "Input:  " << S1 << ", K = " << K1 << "\n";
    std::cout << "Output: " << largest_subsequence(S1, K1) << "\n\n";

    std::string S2 = "3141592653589793238462643383279";
    int K2 = 10;
    std::cout << "Input:  " << S2 << ", K = " << K2 << "\n";
    std::cout << "Output: " << largest_subsequence(S2, K2) << "\n";

    std::string S3 = "98765";
    int K3 = 2;
    std::cout << "Input:  " << S3 << ", K = " << K3 << "\n";
    std::cout << "Output: " << largest_subsequence(S3, K3) << "\n";

    return 0;
}
