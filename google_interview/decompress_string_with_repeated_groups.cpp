/**
Write a function that decompresses a string:

"a(bcd){3}e" -> "abcdbcdbcde"
"a(bc(d){4}e){3}f" -> "abcddddebcddddebcddddef"

This problem asks you to decompress strings with repeated grouped
segments such as <code>(…){k}</code>, including nested groups. For example,
<code>a(bc(d){4}e){3}f</code>
requires expanding the inner repetition first, then repeating the outer
block three times. A typical solution uses recursion or a stack to parse
the string, build substrings for each group, and append repeated content
back into the result. The key challenges are handling nested parentheses,
parsing repeat counts, and assembling the final string correctly.
 */

#include <cctype>
#include <iostream>
#include <string>

class StringDecompressor {
public:
    std::string decompress(cosnt std::string& str) {
        uint32_t idx = 0;
        return decode(str, idx);
    }
private:
    std::string decode(const std::string& str, uint32_t& idx) {
        std::string result = "";
        // Continue processing until we hit the end of the string
        // or a closing parenthesis
        while (idx < str.length() && str[idx] != ')') {
            if (str[idx] == '(') {
                // Start a group
                ++idx;
                // Recursively decode whatever is inside the new group
                const std::string inner = decode(str, idx);
                // Move past the closing ')' - end of group
                ++idx;
                // Parse the repetition count inside {}
                uint32_t repeat_num = 0;
                if (idx < str.length() && str[idx] == '{') {
                    // Move index past '{'
                    ++idx;
                    // Build the number - handles multi-digit
                    while (idx < str.length() && std::isdigit(str[idx])) {
                        repeat_num = repeat_num * 10 + (str[idx] - '0');
                        ++idx; // Move index to the next position
                    }
                    // Found a non-digit, must be a '}', so move to the next position
                    ++idx;
                }
                // Append the recursively decoded string <repeat_num> times
                for (uint32_t r = 0; r < repeat_num; ++r) {
                    result += inner;
                }
            } else {
                // Standard character - append it to the result and move forward
                result += str[i];
                ++i;
            }
        }
        return result;
    }
};

// Demonstration
int main() {
    StringDecompressor decompressor;
    
    std::string test1 = "a(bcd){3}e";
    std::string test2 = "a(bc(d){4}e){3}f";
    
    std::cout << "Input:  " << test1 << "\n";
    std::cout << "Output: " << decompressor.decompress(test1) << "\n\n";
    
    std::cout << "Input:  " << test2 << "\n";
    std::cout << "Output: " << decompressor.decompress(test2) << "\n";
    
    return 0;
}
