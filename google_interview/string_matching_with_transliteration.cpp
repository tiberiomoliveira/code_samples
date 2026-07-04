/**
# String Matching With Transliteration
# You may or may not know that Google Docs has a feature to find and replace even for characters 
# not in the standard English alphabet ([A-Z]). To do this, we first need a transliteration map, e.g.
 
# Keys are single non-standard-English characters, values are
# strings consisting of standard English characters.
 
tl_map = {
    "ë": "e",
    "é": "e",
    "û": "u",
    "æ": "ae",
    "ž": "zed",   # Made up, meant as a harder case for testing.
    ...
}
 
1. Given a haystack string and a needle string and the transliteration mapping,
   return whether the needle is matched within the haystack.
 
Example 1:
haystack: "I love crème brûlée"
needle:   "eme"
return:   True
 
Example 2:
haystack: "I love crème brûlée"
needle:   "ême"
return:   True
 
Example 3:
haystack: "Ole Gunnar Solskjær is amazing"
needle:   "Solskja"
return:   True
 
Example 4:
haystack: "Ole Gunnar Solskjær is amazing"
needle:   "Solskjear"
return:   False
This Google problem asks you to determine whether a "needle" appears in a "haystack",
but with a twist: non-standard characters such as é, ë, æ, or å must be converted
according to a given transliteration map before matching. The map can map one character
to multiple English characters (e.g., "æ" → "ae"), so matching is not just one-to-one
replacement but requires expanding characters properly before comparing segments. The
challenge lies in performing transliteration consistently for both haystack and
needle — while keeping the algorithm efficient enough for large strings.
 */

#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>

std::string get_next_utf8_char(const std::string& str, size_t& idx) {
    uint8_t c = str[idx];
    size_t len = 1;
    // Determine the byte length of the UTF-8 char by checking the leading bits
    if ((c & 0xE0) == 0xC0) {
        len = 2; // 110xxxxx
    } else if ((c & 0xF0) == 0xE0) {
        len = 3; // 1110xxxx
    } else if ((c & 0xF8) == 0xF0) {
        len = 4; // 11110xxx
    }
    // Safety check against malformed UTF-8 strings
    if (idx + len > str.length()) {
        // Adjust the substring length based on the index and string length
        len = str.length() - idx;
    }
    std::string uchar = str.substr(idx, len);
    idx += len; // Update index
    return uchar;
}

// Normalizes a string based on the transliteration map
std::string transliterate(const std::string& input,
                          const std::unordered_map<std::string, std::string>& tl_map) {
    size_t idx = 0;
    std::string result;
    // Pre-allocate memory to ~optimize performance
    result.reserve(input.length());
    while (idx < input.length()) {
        const std::string uchar = get_next_utf8_char(input, idx);
        // If the char exists in the map, append its english equivalent
        auto it = tl_map.find(uchar);
        if (it != tl_map.end()) {
            result += it->second;
        } else {
            // Otherwise, append the original char
            result += uchar;
        }
    }
    return result;
}

// Function to check for matches
bool string_match_with_transliteration(const std::string& haystack,
                                       const std::string& needle,
                                       const std::unordered_map<std::string, std::string> tl_map) {
    // Edge cases
    if (needle.empty()) return true;
    if (haystack.empty()) return false;
    // Normalize both strings
    std::string norm_haystack = transliterate(haystack, tl_map);
    std::string norm_needle = transliterate(needle, tl_map);
    // Search for needle in the haystack
    return norm_haystack.find(norm_needle) != std::string::npos;
}

// --- Testing the Implementation ---
int main() {
    std::unordered_map<std::string, std::string> tl_map = {
        {"ë", "e"},
        {"é", "e"},
        {"û", "u"},
        {"æ", "ae"},
        {"ž", "zed"}
    };

    std::string haystack1 = "I love crème brûlée";
    std::cout << "Example 1: " << stringMatchWithTransliteration(haystack1, "eme", tl_map) << "\n"; // True (1)
    std::cout << "Example 2: " << stringMatchWithTransliteration(haystack1, "ême", tl_map) << "\n"; // True (1)

    std::string haystack2 = "Ole Gunnar Solskjær is amazing";
    std::cout << "Example 3: " << stringMatchWithTransliteration(haystack2, "Solskja", tl_map) << "\n"; // True (1)
    std::cout << "Example 4: " << stringMatchWithTransliteration(haystack2, "Solskjear", tl_map) << "\n"; // False (0)

    return 0;
}

