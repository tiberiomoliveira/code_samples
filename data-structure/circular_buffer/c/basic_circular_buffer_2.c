/**
Implement a fixed-capacity circular ring buffer in C that supports push and pop.
It must be safe for use from a single ISR (producer) and a single main-loop thread (consumer) without a mutex.

Constraints and considerations:
O(1) push and pop.
Memory: exactly N elements.
Use volatile head and tail indices.
Power-of-2 capacity enables fast modulo via bitwise AND.
SPSC (Single-Producer, Single-Consumer) is safe without locks because head is owned exclusively by the producer and tail by the consumer.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// Capacity MUST be a power of 2, so it enables (index & MASK) instead of (index % N)
#define BUFF_CAPACITY  64u    // must be power of 2
#define BUFF_MASK      (BUFF_CAPACITY - 1u)

typedef struct {
    uint8_t          data[BUFF_CAPACITY];
    volatile uint32_t head;   // written only by producer (ISR)
    volatile uint32_t tail;   // written only by consumer (main)
} RingBuf;

uint32_t buffer_size(const RingBuf* rb) {
    return (rb->head - rb->tail) & BUFF_MASK;
}

bool buffer_empty(const RingBuf* rb) {
  return rb->head == rb->tail;
}

bool buffer_full(const RingBuf* rb)  {
    // If full, leave 1 slot empty as a sentinel
    // This AND operation only works if buffer capacity is a power of 2
    return ((rb->head + 1u) & BUFF_MASK) == rb->tail;
}

// Call from ISR only (producer)
bool buffer_push(RingBuf* rb, uint8_t byte) {
    if (buffer_full(rb) {
        return false;
    }
    rb->data[rb->head] = byte;
     // Only possible if the buffer capacity is a power of 2
    const uint32_t next = (rb->head + 1u) & BUFF_MASK;
    // Volatile write, then it will be visible to the consumer
    rb->head = next;
    return true;
}

// Call from main loop (consumer)
bool buffer_pop(RingBuf* rb, uint8_t* out) {
    if (buffer_empty(rb) {
        return false;
    }
    *out = rb->data[rb->tail];
    // Only possible if the buffer capacity is a power of 2
    const uint32_t next = (rb->tail + 1u) & BUFF_MASK
    // Volatile write, then it will be visible to the producer
    rb->tail = next;
    return true;
}

// UART ISR example
// void UART_IRQHandler(void) {
//     uint8_t byte = UART->DR;        // read received byte
//     buffer_push(&uart_rx_buf, byte);  // ISR is producer
// }
//
// void main_loop(void) {
//     uint8_t byte;
//     while (buffer_pop(&uart_rx_buf, &byte)) {
//         process_byte(byte);          // main loop is consumer
//     }
// }
