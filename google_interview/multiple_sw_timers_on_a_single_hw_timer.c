/**
We have an embedded environment with a very simple operating system.
Part of the facilities provided to us is a single hardware timer.
The goal is to implement multiple timers, each with its own callback,
based on this single timer. The hardware timer can be set with the
following function:
void set_hw_timer(int relative_timeout_ms);
After relative_timeout_ms milliseconds have elapsed, the timer triggers
a hardware interrupt, which will call the function:
void handle_hw_timer();
Only one hardware timer may be pending. Calling set_hw_timer again
before the timer has expired will reset the expiration to the new
value without triggering the interrupt. There is also a utility function:
int64_t get_current_time();
which returns the time in milliseconds that has elapsed since boot.
We would like to allow an arbitrary number of timers to be pending.
Define the timer structure and implement this function:
void set_timer(struct timer *t, int relative_timeout_ms, void (*callback)());
Each call to set_timer with a different t parameter should yield a
different timer. The timer structure will be allocated by the caller
and passed in, but the caller will not touch the contents of it, and
you may add any fields you want to it. The callback function should
be called after relative_timeout_ms milliseconds have elapsed. It will
be called in an interrupt context. You will also need to implement the
body of the handle_hw_timer function.
---
This Google interview problem comes from an embedded setting where you
only have one hardware timer, but you need to support many logical timers,
each with its own callback. You are given low-level primitives set_hw_timer
and get_current_time, plus an interrupt handler handle_hw_timer. You must
design a timer struct, implement set_timer so that multiple independent
timers can be scheduled, and write handle_hw_timer to manage a data structure
(often a min-heap or sorted list) that always arms the hardware timer for
the next earliest expiration and invokes the right callbacks in an interrupt context.
*/

#include <stdint.h>

struct timer
{
    int64_t       expiration_time;
    void         (*callback)(void);
    struct timer *next;
};

// Timer sorted linked list
static struct timer *timer_ll = NULL;

/*
 * Helper: Program the hardware timer for the next pending expiry.
 * Must be called with the critical section held (heap must not change under
 * us while we read the linked list).
 */
static void program_next_timer(void)
{
    if (timer_ll == NULL)
        return;

    const int64_t now = get_current_time();
    const int64_t delta = timer_ll->expiration_time - now;

    if (delta < 1)
        delta = 1; // guard: probably it must be positive for the HW call

    set_hw_timer((int)delta);
}

/**
 * set_timer – schedule (or reschedule) a logical timer.
 *
 * @t                   Caller-allocated timer struct. The caller must not
 *                      free or touch it until the callback fires.
 * @relative_timeout_ms Milliseconds from now at which to invoke callback.
 * @callback            Function called in interrupt context at expiry.
 *
 * Calling set_timer on a timer that is already pending silently reschedules
 * it to the new deadline.
 *
 * Complexity: O(n)
 */
void set_timer(struct timer *t,
               int relative_timeout_ms,
               void (*callback)(void))
{
    const int64_t now = get_current_time();

    t->expiration_time = now + relative_timeout_ms;
    t->callback = callback;
    t->next = NULL;

    // Insert into sorted linked list.
    if (timer_ll == NULL ||
        t->expiration_time < timer_ll->expiration_time)
    {
        t->next = timer_ll;
        timer_ll = t;
        // Earliest timer changed.
        program_next_timer();
        return;
    }

    struct timer *cur = timer_ll;
    // Search for the next timer that is greater
    // than the one being added to sort the list.
    while (cur->next &&
           cur->next->expiration_time <= t->expiration_time)
    {
        cur = cur->next;
    }
    // Insert timer into the sorted linked list.
    t->next = cur->next;
    cur->next = t;
}

/**
 * handle_hw_timer – hardware interrupt service routine.
 *
 * Called automatically by the hardware after the delay set by set_hw_timer
 * expires. Execute all callbacks whose deadlines have been reached, then
 * re-program the hardware for the next earliest pending timer.
 *
 * All execution here is in an interrupt context – callbacks must be kept short.
 *
 * Complexity: O(k) where k = number of expired timers.
 */
void handle_hw_timer(void)
{
    // Need to implement `enter_critical()` function
    const int token = enter_critical();
    const int64_t now = get_current_time();
    // Search for expired timers and execute their callbacks.
    while (timer_ll && timer_ll->expiration_time <= now)
    {
        struct timer *expired = timer_ll;
        timer_ll = expired->next;
        expired->callback();
    }

    program_next_timer();
    // Need to implement `exit_critical()` function
    exit_critical();
}

/**
More to the interview:

1. The hardware timer is always armed for the earliest expiration.
2. Timers are stored in a priority structure ordered by expiration time.
3. A min-heap improves insertion to:
    set_timer     O(log n)
    handle        O(log n) // O(k * log n) in total
   while a sorted linked list gives:
    set_timer     O(n)
    handle        O(1) to find next timer // O(k) in total
4. In a real embedded system, access to the timer list must be protected by
disabling interrupts (or another synchronization mechanism) while modifying
it, because set_timer() and handle_hw_timer() can run concurrently.

This synchronization concern is often what interviewers expect candidates
to mention after presenting the basic solution.
*/
