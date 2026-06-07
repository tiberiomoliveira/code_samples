/**
Given a string, return a new string of equal length with the same
characters rearranged in decreasing order of number of occurrences. In
the case of two or more distinct characters occurring the same number
of times, order them by increasing character code.
 
This is a basic test of programming language familiarity with
little reliance on the runtime library. It also requires basic algorithm construction, but nothing fancy. 
 
Hint:
 
Example: given "This is a sample string",
        return "ssssiiiaaTeghlmnprt" 

This task requires you to sort characters in a string by:

Rules:
- Frequency descending 
- If frequencies tie → character code ascending
- Output length must match input length 

Approach 
- Count character frequencies using a map 
- Sort characters by (−frequency, +ASCII) 
- Reconstruct the output string by repeating each character according to its count 

This tests basic algorithm design and comfort with the language, without relying heavily on advanced libraries.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    uint8_t ch;
    int count;
} Entry;

static int compareEntries(const void *a, const void *b)
{
    const Entry *e1 = (const Entry *)a;
    const Entry *e2 = (const Entry *)b;

    // Frequency descending
    if (e1->count != e2->count)
        return e2->count - e1->count;

    // ASCII ascending
    return (int)e1->ch - (int)e2->ch;
}

char *rearrangeByFrequency(const char *input)
{
    int freq[256] = {0};
    const size_t len = strlen(input);

    // Count frequencies
    for (size_t i = 0; i < len; ++i)
    {
        const uint8_t c = (uint8_t)input[i];
        ++freq[c];
    }

    // Build entries
    Entry entries[256];
    int numEntries = 0;

    // O(n)
    for (int i = 0; i < 256; i++)
    {
        if (freq[i] > 0)
        {
            entries[numEntries].ch = (uint8_t)i;
            entries[numEntries].count = freq[i];
            ++numEntries;
        }
    }

    // O(k log k) = O(1), since k is the number of ASCII characters (256)
    qsort(entries,
          numEntries,
          sizeof(Entry),
          compareEntries);

    /* Construct result */
    char *result = (char *)malloc(len + 1);
    int pos = 0;

    for (int i = 0; i < numEntries; ++i)
    {
        for (int j = 0; j < entries[i].count; ++j)
        {
            result[++pos] = entries[i].ch;
        }
    }

    result[pos] = '\0';

    return result;
}

int main(void)
{
    const char *s = "This is a sample string";

    char *result = rearrangeByFrequency(s);

    printf("%s\n", result);

    free(result);

    return 0;
}
