/**
Solve the following interview question using C or C++, whichever
is more convenient:
“””
You and a friend have received a special gold chain as a gift.
The chain links each have an integer weight, not necessarily the same.
You and your friend must choose one of the links to be removed and
provided to charity, after which the chain will be reconnected.
After that, you can choose one place along the chain to split it in two,
such that it creates two equally-weighted sections for you and your friend to take home.
For a given input chain (list of link weights), determine if a solution is possible.
“””
In this Google interview problem, each link of the gold chain has an integer weight.
First, you must remove exactly one link as a donation, reconnect the remaining chain,
and then cut it once so that the two resulting segments have the same total weight.
Given the list of link weights, you need to decide whether there exists some link to
donate and some cut position that makes such an equal split possible. The challenge
is to reason over all choices efficiently rather than brute forcing every
remove-and-cut combination.
 */

#include <iostream>
#include <vector>
#include <numeric>
#include <unordered_map>
#include <unordered_set>

bool canSplitChain(const std::vector<int>& A) {
    int n = A.size();
    
    // A chain must have at least 3 links to remove 1 
    // and split the remainder into 2 non-empty sections.
    if (n < 3) return false; 

    // Use int64_t to prevent potential integer overflow during summation
    int64_t total_sum = 0;
    for (int weight : A) {
        total_sum += weight;
    }

    // Hash sets/maps to store prefix and suffix sums for O(1) lookups
    std::unordered_set<int64_t> left_prefixes;
    std::unordered_map<int64_t, int> right_suffixes;

    // Populate right_suffixes for the initial state (i = 0)
    // Suffixes available will start from index 1 to n-1
    int64_t current_suffix = 0;
    for (int j = n - 1; j >= 1; --j) {
        current_suffix += A[j];
        right_suffixes[current_suffix]++;
    }

    int64_t current_prefix = 0;

    // Iterate through the chain, treating each index 'i' as the donated link
    for (int i = 0; i < n; ++i) {
        int64_t remaining_sum = total_sum - A[i];
        
        // Step 1: Check if the remaining chain can be halved equally
        if (remaining_sum % 2 == 0) {
            int64_t target = remaining_sum / 2;
            
            // Step 2: Check if a valid prefix or suffix matches the target
            if (left_prefixes.count(target) || (right_suffixes[target] > 0)) {
                return true; 
            }
        }

        // Step 3: Shift 'i' to the next element. 
        // Update tracking structures for the next iteration (i + 1)
        current_prefix += A[i];
        left_prefixes.insert(current_prefix);

        // The suffix starting at i + 1 is no longer to the right of our next look
        if (i + 1 < n) {
            int64_t suffix_to_remove = total_sum - current_prefix;
            right_suffixes[suffix_to_remove]--;
            if (right_suffixes[suffix_to_remove] == 0) {
                right_suffixes.erase(suffix_to_remove);
            }
        }
    }

    return false;
}

int main() {
    // Example 1: Remove 4 (index 3). Remaining: [1, 2, 3, 6] -> Split into [1,2,3] and [6]
    std::vector<int> chain1 = {1, 2, 3, 4, 6}; 
    std::cout << std::boolalpha << "Chain 1: " << canSplitChain(chain1) << "\n"; // Expected: true

    // Example 2: Remove 10 (index 0). Remaining: [1, 2, 3] -> Split into [1,2] and [3]
    std::vector<int> chain2 = {10, 1, 2, 3};
    std::cout << "Chain 2: " << canSplitChain(chain2) << "\n"; // Expected: true

    // Example 3: Impossible to split equally after any single removal
    std::vector<int> chain3 = {1, 10, 1};
    std::cout << "Chain 3: " << canSplitChain(chain3) << "\n"; // Expected: false

    return 0;
}
