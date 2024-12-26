#ifndef RF_MODEM_H
#define RF_MODEM_H

#include "irq.h"  // For interrupt handling

// Define IRQ for the RF module
#define IRQ_RF_MODULE 42 

// Structure to represent the RF module device
struct rf_module_device {
    uint8_t mode;  // Mode of the RF module 
};

void rf_module_irq_handler(void *data); // forward decl.

// Declaration of the function to set the mode
static inline void rf_module_set_mode(struct rf_module_device *device, uint8_t mode);

// Implementation of the function to set the mode
static inline void rf_module_set_mode(struct rf_module_device *device, uint8_t mode) {
    device->mode = mode;  // Set the mode of the RF module
}

// Function to initialize the RF module
static inline void rf_module_init(struct rf_module_device *device) {
    rf_module_set_mode(device, 0);  // set mode to RX by default
    printk("RF module initialized\n");  // Log initialization
}

// Function to register the interrupt handler for the RF module
static inline void rf_module_irq_register(struct rf_module_device *device) {
    irq_set_handler(IRQ_RF_MODULE, rf_module_irq_handler, device, 1);  // 1 - priority
    printk("RF module IRQ handler registered\n");  // Log registration
}

// Example interrupt handler for the RF module
void rf_module_irq_handler(void *data) {
    struct rf_module_device *device = (struct rf_module_device *)data;
    printk("RF module IRQ handled\n");  // Log interrupt handling
}

#endif // RF_MODEM_H
