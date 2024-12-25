#include "mouse.h"
#include "io.h"
#include <stdint.h>
#include <stdbool.h>

#define MOUSE_PORT 0x60    // Port to read data from the mouse
#define MOUSE_CMD_PORT 0x64 // Port to send commands to the mouse

// Mouse button states
#define LEFT_BUTTON  0x1
#define RIGHT_BUTTON 0x2
#define MIDDLE_BUTTON 0x4

// Structure to store the current state of the mouse
static MouseState current_mouse_state = {0, 0, 0};

// Initialize the PS/2 port and the mouse
void mouse_init() {
    // Send command to enable mouse interrupts
    outb(MOUSE_CMD_PORT, 0xA8);  // Enable mouse interrupts
    outb(MOUSE_CMD_PORT, 0x20);  // Request current mouse state
    uint8_t status = inb(MOUSE_PORT); 
    status |= 2;  // Set the enable command bit
    outb(MOUSE_CMD_PORT, 0x60);  // Send the new state
    outb(MOUSE_PORT, status);    // Set the new state
}

// Read data from the mouse
static uint8_t mouse_read_data() {
    return inb(MOUSE_PORT);
}

// Mouse interrupt handler
void mouse_interrupt_handler() {
    static uint8_t packet[3];  // Buffer to store mouse data
    static int packet_index = 0;

    uint8_t data = mouse_read_data();

    // Fill the packet with data from the mouse
    packet[packet_index] = data;
    packet_index++;

    // When the 3-byte packet is complete, process the data
    if (packet_index == 3) {
        // Interpret the packet data
        current_mouse_state.buttons = packet[0] & 0x07;  // The first 3 bits represent the buttons
        current_mouse_state.x = (int8_t)packet[1];       // X-axis movement
        current_mouse_state.y = (int8_t)packet[2];       // Y-axis movement

        // Update the mouse state
        // You can add logic to handle mouse movement or button presses

        // Reset packet index for the next packet
        packet_index = 0;
    }
}

// Get the current state of the mouse
MouseState mouse_get_state() {
    return current_mouse_state;
}
