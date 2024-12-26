#ifndef SPI_H
#define SPI_H

#include <stdint.h>
#include "io.h" // For I/O port operations
#include "kernel.h"

// SPI Control Registers and Settings
#define SPI_CTRL_REG         0x4000
#define SPI_STATUS_REG       0x4004
#define SPI_DATA_REG         0x4008
#define SPI_CS_PIN           10  // Chip Select Pin for the SPI device

// SPI Status Flags
#define SPI_STATUS_TX_READY  0x01
#define SPI_STATUS_RX_READY  0x02

// Initialize the SPI interface
static inline void spi_init(void) {
    // Set SPI control registers to default settings
    outb(SPI_CTRL_REG, 0x00);  // Reset SPI control register
    outb(SPI_CTRL_REG, 0x01);  // Enable SPI and configure settings
    printk("SPI initialized\n");
}

// Transfer a byte of data via SPI
static inline void spi_transfer(uint8_t cs_pin, uint8_t *data, size_t len) {
    // Assert chip select
    outb(SPI_CS_PIN, 0); // Activate the chip select
    
    for (size_t i = 0; i < len; i++) {
        // Wait until the SPI is ready for transmission
        while (!(inb(SPI_STATUS_REG) & SPI_STATUS_TX_READY)) {
            // Wait for TX ready flag (example)
        }

        // Write data to SPI data register
        outb(SPI_DATA_REG, data[i]);

        // Wait until the data has been received
        while (!(inb(SPI_STATUS_REG) & SPI_STATUS_RX_READY)) {
            // Wait for RX ready flag (example)
        }

        // Read the received data (if any)
        data[i] = inb(SPI_DATA_REG);
    }

    // Deassert chip select
    outb(SPI_CS_PIN, 1); // Deactivate the chip select

    printk("SPI data transfer complete\n");
}

#endif // SPI_H
