/**
An arithmetic sequence is a list of numbers with a definite pattern.
If you take any number in the sequence and subtract it from the previous one,
the difference is always a constant.

A good arithmetic sequence is an arithmetic sequence with a common difference of either 1 or -1.

For example, [4, 5, 6] is a good arithmetic sequence.
So is [6, 5, 4], [10, 9], or [-3, -2, -1].
But, [1, 2, 1] (no common difference) or [3, 7] (common difference is 4) is NOT.
It implies that any sequence that has only one element is a good arithmetic sequence.

For example, [4] is a good arithmetic sequence.
Given an integer array nums, return the sum of the sums of each subarray that is a good arithmetic sequence.

Example:
Given nums = [7, 4, 5, 6, 5]. Each of the following subarrays is a good arithmetic sequence:
[7], [4], [5], [6], [5],
[4, 5], [5, 6], [6, 5],
[4, 5, 6]

The sums of these subarrays are:
7, 4, 5, 6, 5,
4 + 5 = 9, 5 + 6 = 11, 6 + 5 = 11,
4 + 5 + 6 = 15

Thus, the answer is the sum of all the sums above, which is:
7 + 4 + 5 + 6 + 5 + 9 + 11 + 11 + 15 = 73

This Google interview problem asks you to find all subarrays that form a "good arithmetic sequence",
meaning each adjacent pair differs by exactly +1 or −1 (single elements count as well). For every such
subarray, you compute its sum, then accumulate the sums of all valid subarrays. The challenge is to
detect these patterns efficiently over the whole array and avoid an O(n³) brute force solution.
 */

#include <cstdint>
#include <vector>
using namespace std;

int64_t sumGoodArithmeticSubarrays(const vector<int>& nums)
{
    const int n = nums.size();

    if (n == 0)
        return 0;

    vector<long long> incCount(n);
    vector<long long> decCount(n);

    vector<long long> incSum(n);
    vector<long long> decSum(n);

    incCount[0] = decCount[0] = 1;
    incSum[0] = decSum[0] = nums[0];

    uint64_t totalSum = nums[0];

    // O(n)
    for (int i = 1; i < n; ++i)
    {
        // incremental sequence
        if (nums[i] - nums[i - 1] == 1)
        {
            // length of maximal +1 chain ending at i
            // num: [4  5   6 ]
            // inc: [1 1+1 2+1]
            incCount[i] = incCount[i - 1] + 1;
            // sum of sums of all increasing-good subarrays ending at i
            incSum[i] =
                incSum[i - 1] // extending increasing-good subarray ending at i-1
                + incCount[i - 1] * static_cast<int64_t>(nums[i]) // extending maximal chaing adds nums[i] to each one
                + nums[i]; // adds self subarray
        }
        else
        {
            // sum itself only
            incCount[i] = 1;
            incSum[i] = nums[i];
        }
        // Decremental sequence
        if (nums[i] - nums[i - 1] == -1)
        {
            // length of maximal -1 chain ending at i
            // num: [4  5   6 ]
            // inc: [1 1+1 2+1]
            decCount[i] = decCount[i - 1] + 1;
            // sum of sums of all increasing-good subarrays ending at i
            decSum[i] =
                decSum[i - 1] // extending increasing-good subarray ending at i-1
                + decCount[i - 1] * static_cast<int64_t>(nums[i]) // extending maximal chaing adds nums[i] to each one
                + nums[i]; // adds self subarray
        }
        else
        {
            // sum itself only
            decCount[i] = 1;
            decSum[i] = nums[i];
        }
        // sum all inc and dec contributions
        totalSum += incSum[i];
        totalSum += decSum[i];
        totalSum -= nums[i]; // remove duplicate (decSum and incSum) singleton
    }

    return totalSum;
}
