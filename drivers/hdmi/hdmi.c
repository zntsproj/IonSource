// All these headers are from /include/ path 
#include "hdmi.h"
#include "kernel.h"

// Initialize the HDMI device
int hdmi_init(struct hdmi_device *dev, uint32_t base_addr, uint8_t irq) {
    if (!dev) {
        printk("Error: NULL device pointer\n");
        return -1;
    }

    dev->base_addr = base_addr;
    dev->irq = irq;

    // Initialize HDMI device registers
    outl(dev->base_addr + 0x00, 0x00000000);  // Reset or setup register
    outl(dev->base_addr + 0x04, 0x00000001);  // Enable HDMI

    printk("HDMI device initialized at base address 0x%X\n", dev->base_addr);

    // If IRQ is needed, register handler
    if (irq != 0xFF) {
        irq_set_handler(irq, hdmi_irq_handler, dev, 0);  // Low priority
        irq_enable(irq);
    }

    return 0;
}

// Set the resolution of the HDMI output
int hdmi_set_resolution(struct hdmi_device *dev, uint32_t width, uint32_t height) {
    if (!dev) {
        printk("Error: NULL device pointer\n");
        return -1;
    }

    outl(dev->base_addr + 0x10, width);
    outl(dev->base_addr + 0x14, height);

    printk("HDMI resolution set to %dx%d\n", width, height);
    return 0;
}

// Enable the HDMI output
void hdmi_enable_output(struct hdmi_device *dev) {
    if (!dev) {
        printk("Error: NULL device pointer\n");
        return;
    }

    // Set register to enable output (example register)
    outl(dev->base_addr + 0x18, 0x01);  // Enable HDMI output

    printk("HDMI output enabled\n");
}

// Disable the HDMI output
void hdmi_disable_output(struct hdmi_device *dev) {
    if (!dev) {
        printk("Error: NULL device pointer\n");
        return;
    }

    // Set register to disable output
    outl(dev->base_addr + 0x18, 0x00);  // Disable HDMI output

    printk("HDMI output disabled\n");
}
