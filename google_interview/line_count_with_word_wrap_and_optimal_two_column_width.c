/**
Given an English-language text and the width of a page, find how many
lines the text would take on that page.

Given a text that you’d like to place into a two-column table on a
fixed-width page, find the best column width that minimizes the overall
table height.

Subproblem 1 (line count): Given page width, compute the number of
lines under word-wrapping with no hyphenation. Greedy pack words;
start a new line when the next word doesn’t fit. Runs in O(n). 

Subproblem 2 (two-column optimum): For a fixed page width, choose equal
column width to minimize total height. For a width w, wrap to get total
lines L(w); two-column height is ceil(L(w)/2). Since H(w) is non-increasing
with w, the optimal choice is the largest feasible w satisfying layout
constraints (2*w + gutter ≤ pageWidth). If widths are integral pixels,
do a binary search or a simple sweep.
 */

#include <stdio.h>
#include <string.h>

// Assume the text is already tokenized into words.
int countLines(char *words[], int numWords, int width)
{
    if (numWords == 0)
        return 0;

    int lines = 1;
    int currentLen = strlen(words[0]);

    for (int i = 1; i < numWords; ++i)
    {
        const int len = strlen(words[i]);

        if (currentLen + 1 + len <= width)
        {
            currentLen += 1 + len;
        }
        else
        {
            ++lines;
            currentLen = len;
        }
    }

    return lines;
}

int bestColumnWidth(int pageWidth, int gutter)
{
    // The optimal column width is simply the largest feasible width
    // because larger widths never worsen the height.
    // No sophisticated optimization is actually required.
    return (pageWidth - gutter) / 2;
}

int computeTableHeight(char *words[],
                       int numWords,
                       int pageWidth,
                       int gutter)
{
    const int width = bestColumnWidth(pageWidth, gutter);
    const int lines = countLines(words, numWords, width);

    return (lines + 1) / 2;
}
