/**
You are given a list of integers A and an integer N. Implement a functioncountTriplets(A, N) that returns the number of unique triplets that can be created from A whose elements sum to ≤ N.
Triplets are order-insensitive (so the order you choose the elements doesn’t matter) and elements may include duplicate values if A has elements with the same value, but they can only choose a given index once.
Note that countTriplets() returns a single integer. You should NOT return the entire list of triplets.
Example:
---
A = [-4, 2, 2, -7, 5, 3]
N = 4

countTriplets(A, N) == 16

(-4, 3, 5) == (5, 3, -4)

Examples of valid triplets:
---
(-4, 2, 5)   // Sum = -4 + 2 + 5 = 3 <= N
(-4, 2, 2)   // Sum = -4 + 2 + 2 = 0 <= N

Examples of invalid triplets:
---
(2, 5, 3)    // Sum = 2 + 5 + 3 = 10 > N

From array A, count how many unique index-based triplets have a sum ≤ N.

Triplets are order-insensitive, and duplicates are allowed only if present in A.

Return only the count, not the triplets.

Usual approach: sort + 2-loops + binary search (O(n² log n)) or sort + 2-pointer optimization.
 */

#include <stdio.h>
#include <stdlib.h>

static int compare(const void *a, const void *b)
{
    const int x = *(const int *)a;
    const int y = *(const int *)b;

    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

int64_t countTriplets(int A[], int size, int N)
{
    // O(n log n)
    qsort(A, size, sizeof(int), compare);

    int64_t count = 0;

    // O(n)
    for (int i = 0; i < size - 2; ++i)
    {
        // If A[i]+A[ledt]+A[right] <= N, then all in between are valid too
        int left = i + 1;
        int right = size - 1;

        // O(n)
        while (left < right)
        {
            int64_t sum =
                (int64_t)A[i] +
                (int64_t)A[left] +
                (int64_t)A[right];

            if (sum <= N)
            {
                count += (right - left);
                ++left;
            }
            else
            {
                --right;
            }
        }
    } // O(n^2)

    return count;
}