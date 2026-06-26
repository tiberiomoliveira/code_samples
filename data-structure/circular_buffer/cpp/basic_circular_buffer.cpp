/**
Lock-Free Single-Producer Single-Consumer Circular Buffer

Consideration:
- The buffer size may not be a power of 2
- The buffer can have any type
 */

#include <atomic>
#include <cstddef>

template <typename T, size_t SIZE>
class LockFreeQueue {
private:
    T buffer[SIZE];
    // It is syntactically valid to declare an atomic variable as volatile
    // in C++, but doing so may not be beneficial and can complicate the
    // intended thread safety of the atomic operations.
    std::atomic<size_t> head{0};
    std::atomic<size_t> tail{0};

public:
    bool enqueue(const T& item) {
        cosnt size_t current_tail = tail.load(std::memory_order_relaxed);
        const size_t next_tail = (current_tail + 1) % SIZE;
        if (next_tail == head.load(std::memory_order_acquire)) {
            return false; // Buffer full
        }
        buffer[current_tail] = item;
        tail.store(next_tail, std::memory_order_release);
        return true;
    }

    bool dequeue(T& item) {
        const size_t current_head = head.load(std::memory_order_relaxed);
        if (current_head == tail.load(std::memory_order_acquire)) {
            return false; // Buffer empty
        }
        item = buffer[current_head];
        head.store((current_head + 1) % SIZE, std::memory_order_release);
        return true;
    }
};
