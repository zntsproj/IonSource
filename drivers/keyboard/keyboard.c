#include "keyboard.h"
#include "io.h"

// Port for reading keyboard status
#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_DATA_PORT 0x60

// Function to wait for keyboard data to be available
static void wait_for_keyboard_data() {
    while ((inb(KEYBOARD_STATUS_PORT) & 0x01) == 0) {
        // Wait until data is available in the keyboard port
    }
}

// Read the scan code from the keyboard
uint8_t read_keyboard_data() {
    wait_for_keyboard_data();
    return inb(KEYBOARD_DATA_PORT);  // Read data from port 0x60
}

// Handle key press event
void handle_key_press(uint8_t scancode) {
    // Check if the key was released (bit 7 is set)
    if (scancode & 0x80) {
        // Key has been released (e.g., Shift key)
        uint8_t released_key = scancode & 0x7F;
        // Logic to handle key release (e.g., changing state)
        return;
    }

    // Find the corresponding character or action for this scan code
    uint8_t key = keymap[scancode];
    
    // If it's a regular key, process the character
    if (key != 0) {
        // Logic to process the key press
        // For example, print the key to the screen or process input
        // putchar(key); // Function to output the character to the screen
    } else {
        // This is a special key (e.g., Shift or Ctrl)
        switch (scancode) {
            case KEY_ESC:
                // Handle ESC key
                break;
            case KEY_SHIFT_L:
            case KEY_SHIFT_R:
                // Handle Shift key press
                break;
            case KEY_CTRL_L:
            case KEY_CTRL_R:
                // Handle Ctrl key press
                break;
            case KEY_ALT_L:
            case KEY_ALT_R:
                // Handle Alt key press
                break;
            default:
                break;
        }
    }
}

// Function to initialize the keyboard
void init_keyboard() {
    // This is where we can initialize the keyboard or perform setup tasks.
    // For example, check its readiness or set its operating mode.
    
    // Reset the controller if needed
    outb(KEYBOARD_STATUS_PORT, 0xFF);  // Reset command, if needed to reset the controller.
}
