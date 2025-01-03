#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "io.h"

// Define base I/O ports and register offsets for QCA988X device
#define QCA988X_BASE_PORT    0x5000
#define QCA988X_TX_CTRL      (QCA988X_BASE_PORT + 0x00)
#define QCA988X_RX_CTRL      (QCA988X_BASE_PORT + 0x04)
#define QCA988X_STATUS       (QCA988X_BASE_PORT + 0x08)
#define QCA988X_CHANNEL      (QCA988X_BASE_PORT + 0x0C)
#define QCA988X_MAC_ADDR_L   (QCA988X_BASE_PORT + 0x10)
#define QCA988X_MAC_ADDR_H   (QCA988X_BASE_PORT + 0x14)
#define QCA988X_POWER_CTRL   (QCA988X_BASE_PORT + 0x18)
#define QCA988X_DATA_RATE    (QCA988X_BASE_PORT + 0x1C)

// TX and RX descriptors
typedef struct {
    uint32_t buffer_addr;  // Physical address of the buffer
    uint16_t length;       // Length of the data
    uint16_t flags;        // Flags for transmission/reception
} qca988x_descriptor_t;

#define QCA988X_MAX_DESCRIPTORS 256
static qca988x_descriptor_t tx_descriptors[QCA988X_MAX_DESCRIPTORS];
static qca988x_descriptor_t rx_descriptors[QCA988X_MAX_DESCRIPTORS];

// Set MAC address by writing to the registers
void qca988x_set_mac_address(uint64_t mac_addr) {
    outl(QCA988X_MAC_ADDR_L, (uint32_t)(mac_addr & 0xFFFFFFFF));
    outl(QCA988X_MAC_ADDR_H, (uint32_t)(mac_addr >> 32));
    printf("MAC address set to: %012llX\n", mac_addr);
}

// Initialize the QCA988X device
void qca988x_init() {
    // Reset TX/RX control registers to initial state
    outl(QCA988X_TX_CTRL, 0x0);
    outl(QCA988X_RX_CTRL, 0x0);

    // Set MAC address
    set_mac_address(0x123456789ABC); // Test MAC address

    // Set channel to 36 (5 GHz)
    outl(QCA988X_CHANNEL, 36);

    // Enable power saving
    outl(QCA988X_POWER_CTRL, 1);

    // Set default data rate to 54 Mbps (highest common rate for compatibility)
    outl(QCA988X_DATA_RATE, 54000);

    // Initialize TX descriptors
    for (int i = 0; i < QCA988X_MAX_DESCRIPTORS; i++) {
        tx_descriptors[i].buffer_addr = 0;
        tx_descriptors[i].length = 0;
        tx_descriptors[i].flags = 0;
    }

    // Initialize RX descriptors
    for (int i = 0; i < QCA988X_MAX_DESCRIPTORS; i++) {
        rx_descriptors[i].buffer_addr = 0;
        rx_descriptors[i].length = 0;
        rx_descriptors[i].flags = 0;
    }

    // Enable RX and TX hardware blocks
    outl(QCA988X_RX_CTRL, 1);
    outl(QCA988X_TX_CTRL, 1);

    printf("QCA988X device initialized.\n");
}

// Transmit a packet
int qca988x_transmit(void *data, uint16_t length) {
    for (int i = 0; i < QCA988X_MAX_DESCRIPTORS; i++) {
        if (tx_descriptors[i].flags == 0) { // Descriptor is free
            tx_descriptors[i].buffer_addr = (uint32_t)data;
            tx_descriptors[i].length = length;
            tx_descriptors[i].flags = 1;

            // Trigger transmission by setting the TX control register
            outl(QCA988X_TX_CTRL, 1);
            printf("Packet transmitted, length: %u bytes\n", length);
            return 0; // Success
        }
    }
    return -1; // No available descriptor
}

// Receive a packet
int qca988x_receive(void *buffer, uint16_t *length) {
    for (int i = 0; i < QCA988X_MAX_DESCRIPTORS; i++) {
        if (rx_descriptors[i].flags == 1) { // Descriptor has data
            *length = rx_descriptors[i].length;
            uint32_t addr = rx_descriptors[i].buffer_addr;

            // Copy data to the provided buffer
            memcpy(buffer, (void *)addr, *length);

            rx_descriptors[i].flags = 0;
            printf("Packet received, length: %u bytes\n", *length);
            return 0; // Success
        }
    }
    return -1; // No data available
}

// Check the status of the QCA988X device
void qca988x_check_status() {
    uint32_t status = inl(QCA988X_STATUS);
    printf("Device status: 0x%08X\n", status);
}

// Shutdown the QCA988X device
void qca988x_shutdown() {
    // Disable TX/RX control
    outl(QCA988X_TX_CTRL, 0);
    outl(QCA988X_RX_CTRL, 0);

    // Disable power control
    outl(QCA988X_POWER_CTRL, 0);

    printf("QCA988X device shutdown.\n");
}
