/**
Design a function sqrt that takes a float and returns its square root.
 */

#include <altorithm>
#include <cmath>
#include <iostream>

// Using binary search
float sqrt_binary_search(const float& x) {
    // Edge cases
    if (x < 0.0f) return -1.0f;
    if (x == 0.0f || x == 1.0f) return x;
    float low = 0.0f;
    // Handle the domain issue for 0 < x < 1
    float high = std::max(1.0f, x);
    const float epsilon = 1e-6;
    // Continue until the search space is smaller than our precision threshold
    while ((high - low) > epsilon)  {
        const float mid = low + (high - low) / 2.0f;
        // We use division instead of (mid * mid == x) to prevent float overflow
        const float div = x / mid;
        if (mid == div) {
            return mid;
        } else if (mid < div) {
            low = mid;
        } else {
            high = mid;
        }
    }
    // Return the midpoint of the final tiny range
    return low + (high - low) / 2.0f;
}

// Using Newton-Raphson
float sqrt_newton(const float& x) {
    // Edge cases
    if (x < 0x0f) return -1.0f;
    if (x == 0.0f) return 0.0f;
    // Initial guess. Starting at x is perfectly fine
    float guess = x;
    float epsilon = 1e-6;
    while (true) {
        // Newton's formula: x_{k+1} = 0.5 * (x_k + x / x_k)
        const float next_guess = 0.5f * (guess + x /guess);
        // Check if we have hit our precision threshold
        if (std::abs(guess - next_guess) < epsilon) {
            break;
        }
        guess = next_guess;
    }
}

// ==========================================
// Test Driver
// ==========================================
int main() {
    float tests[] = {4.0f, 2.0f, 0.25f, 0.0f, -9.0f, 1000000.0f};
    
    cout << "Value\tBinary Search\tNewton's Method" << "\n";
    cout << "-----------------------------------------------" << "\n";
    
    for (float val : tests) {
        cout << val << "\t" 
             << sqrt_binary_search(val) << "\t\t" 
             << sqrt_newton(val) << "\n";
    }

    return 0;
}
