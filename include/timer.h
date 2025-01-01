#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

// Timer frequency in Hz
#define TIMER_FREQUENCY 1000

// Structure that describes a timer
typedef struct {
    uint32_t interval_ms;      // Timer interval in milliseconds
    void (*callback)(void);    // Function to call when the timer expires
    uint32_t counter;          // Counter to keep track of elapsed time
    uint8_t is_active;         // Timer status (active or inactive)
} timer_t;

// Function prototypes for timer operations
void timer_init();
void timer_start(timer_t *timer, uint32_t interval_ms, void (*callback)(void));
void timer_stop(timer_t *timer);
void timer_update(); 

// Timer array to store multiple timers
static timer_t timers[10];

// Initialize all timers (by default, all timers are inactive)
void timer_init() {
    for (int i = 0; i < 10; i++) {
        timers[i].is_active = 0;  // Set all timers as inactive
    }
}

// Start a timer with a specified interval in milliseconds and a callback function
void timer_start(timer_t *timer, uint32_t interval_ms, void (*callback)(void)) {
    timer->interval_ms = interval_ms;  // Set the interval for the timer
    timer->callback = callback;        // Set the callback function
    timer->counter = 0;                // Reset the counter
    timer->is_active = 1;              // Mark the timer as active
}

// Stop a timer by marking it as inactive
void timer_stop(timer_t *timer) {
    timer->is_active = 0;  // Mark the timer as inactive
}

// Update all active timers (this function should be called every millisecond)
void timer_update() {
    for (int i = 0; i < 10; i++) {
        if (timers[i].is_active) {
            timers[i].counter++;  // Increment the counter for the active timer
            
            // If the timer's counter reaches the specified interval, trigger the callback
            if (timers[i].counter >= timers[i].interval_ms) {
                timers[i].callback();  // Call the callback function
                timers[i].counter = 0;  // Reset the counter
            }
        }
    }
}

#endif // TIMER_H
