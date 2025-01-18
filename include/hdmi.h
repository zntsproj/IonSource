#ifndef HDMI_H
#define HDMI_H

#include <stdint.h>
#include "io.h"
#include "irq.h"

// Structure to represent the HDMI device
struct hdmi_device {
    uint32_t base_addr;    // Base address of the HDMI device registers
    uint8_t irq;           // IRQ number for the HDMI device
};

// Function to initialize the HDMI device
int hdmi_init(struct hdmi_device *dev, uint32_t base_addr, uint8_t irq);

// Function to set the resolution
int hdmi_set_resolution(struct hdmi_device *dev, uint32_t width, uint32_t height);

// Function to enable HDMI output
void hdmi_enable_output(struct hdmi_device *dev);

// Function to disable HDMI output
void hdmi_disable_output(struct hdmi_device *dev);

// Function to handle HDMI interrupts (if necessary)
void hdmi_irq_handler(void *data);

#endif // HDMI_H
