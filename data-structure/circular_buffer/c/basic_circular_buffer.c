/*
  Problem #1: Circular Buffer Implementation (Bare-Metal, Interrupt-Safe)

You're working on firmware for a microcontroller-based sensor system with no OS, no malloc, and limited RAM.

Your task is to implement a circular buffer (ring buffer) for uint8_t data. The buffer will be written to by an interrupt service routine (ISR) and read from by the main loop.

Requirements:
Implement a fixed-size circular buffer.

Must support:

void buffer_init(uint8_t *buf, size_t size);

bool buffer_write(uint8_t byte); (called from ISR)

bool buffer_read(uint8_t *byte); (called from main loop)

Must be safe under interrupt concurrency, i.e., prevent race conditions.

No dynamic memory allocation.

Max portability: no architecture-specific code unless absolutely required (e.g., disabling/enabling interrupts).
*/

#include <stdint.h>
#include <stdbool.h>

#define MAX_CAPACITY 256

static volatile uint8_t *buffer = NULL;
static volatile int capacity = 0;
static volatile int front = 0;
static volatile int back = 0;

extern void disable_interrupts(void);
extern void enable_interrupts(void);

void buffer_init(uint8_t *buf, int size) {
    if (size > MAX_CAPACITY) size = MAX_CAPACITY;

    disable_interrupts();
    buffer = buf;
    capacity = size;
    front = back = 0;
    enable_interrupts();
}

// Called from ISR
bool buffer_write(uint8_t byte) {
    disable_interrupts();

    int next = (back + 1) % capacity;
    if (next == front)
        front = (front + 1) % capacity;  // Overwrite oldest

    buffer[back] = byte;
    back = next;

    enable_interrupts();
    return true;
}

// Called from main loop
bool buffer_read(uint8_t *byte) {
    disable_interrupts();

    if (front == back) {
        enable_interrupts();
        return false; // Empty
    }

    *byte = buffer[front];
    front = (front + 1) % capacity;

    enable_interrupts();
    return true;
}
