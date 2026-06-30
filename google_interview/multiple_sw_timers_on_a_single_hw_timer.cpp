/**
Solve the following interview question using C or C++, whichever is more convenient:
We have an embedded environment with a very simple operating system. Part of the
facilities provided to us is a single hardware timer.
The goal is to implement multiple timers, each with their own callback, based on
this single timer.
The hardware timer can be set with the following function:
void set_hw_timer(int relative_timeout_ms);
After relative_timeout_ms milliseconds have elapsed, the timer triggers a hardware
interrupt, which will call the function: void handle_hw_timer();
Only one hardware timer may be pending. Calling set_hw_timer again before the timer
has expired will reset the expiration to the new value without triggering the interrupt.
There is also a utility function:
int64_t get_current_time();
which returns the time in milliseconds that have elapsed since boot.
We would like to allow an arbitrary number of timers to be pending. Define the timer
structure and implement this function:
void set_timer(struct timer *t, int relative_timeout_ms, void (*callback)());
Each call to set_timer with a different t parameter should yield a different timer.
The timer structure will be allocated by the caller and passed in, but the caller will
not touch the contents of it and you may add any fields you want to it.
The callback function should be called after relative_timeout_ms milliseconds have elapsed.
It will be called in interrupt context.
You will also need to implement the body of the handle_hw_timer function.

This Google interview problem comes from an embedded setting where you only have one
hardware timer, but you need to support many logical timers, each with its own callback.
You are given low-level primitives set_hw_timer and get_current_time, plus an interrupt
handler handle_hw_timer. You must design a timer struct, implement set_timer so that multiple
independent timers can be scheduled, and write handle_hw_timer to manage a data structure
(often a min-heap or sorted list) that always arms the hardware timer for the next earliest
expiration and invokes the right callbacks in interrupt context.
 */

#include <algorithm>
#include <cstdint>

// Provided
void set_hw_timer(int relative_timeout_ms);
int64_t get_current_time();

// Mocked
void disable_interrupts();
void enable_interrupts();

struct timer {
  int64_t abs_expiry;  // Absolute time when this timer should fire
  void (*callback)();  // Callback function pointer
  struct timer* next;  // Pointer to tne next timer
}

static timer* timer_head = nullptr;

void set_timer(
  struct timer *t,
  int rel_timeout_ms,
  void (*callback)()) {
  // Calculate the absolute time this timer must fire
  t->abs_expiry = get_current_time() + rel_timeout_ms;
  t->callback = callback;
  t->next = nullptr;

  // Start of Critical Section - prevent the interrupt from modifying the list
  disable_interrupts();

  // Case where list is empty, or the new timer expires earlier than the current head
  if (timer_head == nullptr || t->abs_expiry < timer_head->abs_expire) {
    // Update head
    t->next = timer_head;
    timer_head = t;
    // Reprogram hardware timer since this is the absolute earlier timer
    set_hw_timer(rel_timeout_ms);
  } else {
    timer* curr = timer_head;
    // Find correct position for new timer
    while (curr->next != nullptr &&
           curr->abs_expiry <= t->abs_expiry) { // O(N) - N is the number of timers
      curr = curr->next;
    }
    // Insert the timer into its correct position
    t->next = curr->next;
    curr->next = t;
  }
  // End of Critical Section
  enable_interrupt();
}

void handle_hw_timer() {
  // We are in interrupt context
  const int64_t now = get_current_time();
  // Process all expired timers until now
  while (timer_head != nullptr &&
         timer_head->abs_expiry <= now) { // O(K) - K is the expired timers
    timer* expired_timer = timer_head;
    // Update timer singly list head
    timer_head = timer_head->next;
    if (expired_timer->callback != nullptr) {
      // Better if we add this to a queue to process callback outside the interrupt
      expired_timer->callback();
    }
  }
  // If timer singly list is not empty, re-arm the HW timer
  if (timer_head != nullptr) {
    int64_t remaining_timer = timer_head->abs_expiry = get_current_time();
    // Make sure the head's remaining timer is not negative
    if (remaining_timer < 0) {
      remaining_timer = 0;
    }
    // Re-arm timer
    set_hw_timer(static_cast<int>(remaining_timer));
  }
}
