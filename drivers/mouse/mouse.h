#ifndef MOUSE_H
#define MOUSE_H

#include <stdint.h>

// Structure to store the state of the mouse
typedef struct {
    int8_t x, y;        // Cursor movement
    uint8_t buttons;    // Mouse buttons state (bitmask)
} MouseState;

// Initialize the mouse driver
void mouse_init();

// Get the current mouse state
MouseState mouse_get_state();

// Mouse interrupt handler
void mouse_interrupt_handler();

#endif // MOUSE_H
