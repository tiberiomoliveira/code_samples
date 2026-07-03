/**
 */

#include <cstdint>
#include <cstdlib>
#include <iostream>

void* aligned_malloc(size_t size, size_t aligment) {
    // Safety checks: alignment must be a non-zero power of 2 number
    if (alignment == 0 || (alignment & (alignment - 1) != 0) {
        return nullptr;
    }
    // Need extra padding (alignment - 1) and space to store original raw pointer (void*)
    const size_t total_size = size + (alignment - 1) + sizeof(void*);
    void* p_raw = std::malloc(total_size);
    if (p_raw == nullptr) {
        return nullptr;
    }
    // Determine the absolute minimum address where our aligned block can start
    uintprt_t raw_addr = reinterpret_cast<uintptr_t>(p_raw);
    uintptr_t alignment_base = raw_addr + sizeof(void*);
    // Round up alignment_base to the next multiple of 'alignment' using bitwise operation
    uintptr_t aligned_addr = (alignment_base + alignment - 1) & ~(alignment - 1);
    void* p_aligned = reinterpret_cast<void*>(aligned_addr);
    // Save the original pointer in the slot directly preceding the aligned pointer
    (reinterpret_cast<void**>(p_aligned))[-1] = p_raw;
    return p_aligned;
}

void aligned_free(void* aligned) {
    if (aligned == nullptr) {
        return;
    }
    // Set the raw pointer
    void* p_raw = (reinterpret_cast<void**>(p_aligned))[-1];
    std::free(p_raw);
}

int main() {
    size_t size = 100;
    size_t alignment = 64; // Align to a 64-byte cache line

    void* ptr = aligned_malloc(size, alignment);
    
    if (ptr != nullptr) {
        std::cout << "Successfully allocated aligned memory!\n";
        std::cout << "Returned Address: " << ptr << "\n";
        
        // Verify alignment mathematically
        if (reinterpret_cast<uintptr_t>(ptr) % alignment == 0) {
            std::cout << "Verification passed: Address is perfectly aligned.\n";
        } else {
            std::cout << "Verification failed!\n";
        }

        aligned_free(ptr);
        std::cout << "Memory successfully freed.\n";
    }

    return 0;
}
