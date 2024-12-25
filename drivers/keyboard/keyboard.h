#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

// Keyboard ports
#define KEYBOARD_DATA_PORT 0x60  // Port for keyboard data
#define KEYBOARD_STATUS_PORT 0x64 // Port for keyboard status
#define KEYBOARD_ACK 0xFA        // Acknowledgement from keyboard

// Expanded keymap including modifier keys (Shift, Ctrl, Alt) and some function keys
static const uint8_t keymap[256] = {
    [0x1E] = '1', [0x1F] = '2', [0x20] = '3', [0x21] = '4',
    [0x22] = '5', [0x23] = '6', [0x24] = '7', [0x25] = '8',
    [0x26] = '9', [0x27] = '0', [0x2D] = '-', [0x2E] = '=',
    [0x2C] = 'q', [0x2D] = 'w', [0x2E] = 'e', [0x2F] = 'r',
    [0x30] = 't', [0x31] = 'y', [0x32] = 'u', [0x33] = 'i',
    [0x34] = 'o', [0x35] = 'p', [0x36] = '[', [0x37] = ']',
    [0x38] = 'a', [0x39] = 's', [0x3A] = 'd', [0x3B] = 'f',
    [0x3C] = 'g', [0x3D] = 'h', [0x3E] = 'j', [0x3F] = 'k',
    [0x40] = 'l', [0x41] = ';', [0x42] = '\'', [0x43] = '`',
    [0x44] = 'z', [0x45] = 'x', [0x46] = 'c', [0x47] = 'v',
    [0x48] = 'b', [0x49] = 'n', [0x4A] = 'm', [0x4B] = ',',
    [0x4C] = '.', [0x4D] = '/', 
    [0x4E] = ' ', // Spacebar

    // Control keys represented as simple macros or characters
    [0x01] = 0x01,  // ESC key
    [0x2A] = 0x2A,  // Left Shift
    [0x36] = 0x36,  // Right Shift
    [0x1D] = 0x1D,  // Left Control
    [0x1D + 0x80] = 0x1D + 0x80, // Right Control
    [0x38] = 0x38,  // Left Alt
    [0x38 + 0x80] = 0x38 + 0x80, // Right Alt
    [0x3B] = 0x3B,  // F1
    [0x3C] = 0x3C,  // F2
    [0x3D] = 0x3D,  // F3
    [0x3E] = 0x3E,  // F4
    [0x3F] = 0x3F,  // F5
    [0x40] = 0x40,  // F6
    [0x41] = 0x41,  // F7
    [0x42] = 0x42,  // F8
    [0x43] = 0x43,  // F9
    [0x44] = 0x44,  // F10
    [0x57] = 0x57,  // F11
    [0x58] = 0x58,  // F12
};

// Define the keys as characters or numeric values
#define KEY_ESC 0x01
#define KEY_SHIFT_L 0x2A
#define KEY_SHIFT_R 0x36
#define KEY_CTRL_L 0x1D
#define KEY_CTRL_R (0x1D + 0x80)
#define KEY_ALT_L 0x38
#define KEY_ALT_R (0x38 + 0x80)
#define KEY_F1 0x3B
#define KEY_F2 0x3C
#define KEY_F3 0x3D
#define KEY_F4 0x3E
#define KEY_F5 0x3F
#define KEY_F6 0x40
#define KEY_F7 0x41
#define KEY_F8 0x42
#define KEY_F9 0x43
#define KEY_F10 0x44
#define KEY_F11 0x57
#define KEY_F12 0x58

// Function to read data from the keyboard's data port
uint8_t read_keyboard_data();

// Function to handle a key press, mapping scancode to character
void handle_key_press(uint8_t scancode);

#endif // KEYBOARD_H
