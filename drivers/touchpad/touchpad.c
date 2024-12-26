#include "irq.h"       // For interrupt handling
#include "io.h"        // For I/O port operations
#include "kernel.h"    // For printk

// Structure to represent the touchpad device state
struct touchpad_device {
    unsigned int x;   // X coordinate
    unsigned int y;   // Y coordinate
    unsigned int button_state; // Button state (e.g., pressed or not)
};

// Initialize the touchpad device
void touchpad_init(struct touchpad_device *device) {
    // Initialize coordinates and button state (set to 0 by default)
    device->x = 0;
    device->y = 0;
    device->button_state = 0;
    printk("Touchpad device initialized\n");
}

// Example interrupt handler for the touchpad (PS/2 interface)
void touchpad_irq_handler(void *data) {
    struct touchpad_device *device = (struct touchpad_device *)data;

    // Read touchpad data (e.g., X and Y coordinates from ports 0x60 and 0x64)
    unsigned int new_x = inb(0x60);  // Read X coordinate data from touchpad (example)
    unsigned int new_y = inb(0x64);  // Read Y coordinate data from touchpad (example)
    
    // Read button state (example)
    unsigned int button_state = inb(0x60);  // Read button state (example)

    // Update touchpad coordinates and button state
    device->x = new_x;
    device->y = new_y;
    device->button_state = button_state;

    // Print the updated touchpad position and button state
    printk("Touchpad moved to: (%u, %u), Button State: %u\n", device->x, device->y, device->button_state);
}

// Function to register the touchpad interrupt handler
void touchpad_irq_register(struct touchpad_device *device) {
    // Register the interrupt handler for IRQ
    irq_set_handler(12, touchpad_irq_handler, device, 0);  // Using IRQ 12 with default priority 0
    printk("Touchpad IRQ handler registered\n");
}

// Main touchpad driver initialization
void touchpad_driver_init(void) {
    struct touchpad_device touchpad;

    // Initialize the touchpad device
    touchpad_init(&touchpad);

    // Register the interrupt handler for the touchpad
    touchpad_irq_register(&touchpad);
}

// Main kernel function
int main(void) {
    // Initialize the interrupt system
    irq_init();
    printk("IRQ system initialized\n");

    // Initialize the touchpad driver
    touchpad_driver_init();

    return 0;
}
