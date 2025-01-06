#include "io.h"
#include <stdint.h>
#include <stdio.h>

#define IR_BASE_ADDR 0x40000000

#define IR_CR      (*(volatile uint32_t *)(IR_BASE_ADDR + 0x00))  // IR Control Register
#define IR_SR      (*(volatile uint32_t *)(IR_BASE_ADDR + 0x04))  // IR Status Register
#define IR_IRIMR   (*(volatile uint32_t *)(IR_BASE_ADDR + 0x08))  // IR Interrupt Mask Register
#define IR_IRIER   (*(volatile uint32_t *)(IR_BASE_ADDR + 0x0C))  // IR Interrupt Enable Register
#define IR_IRBUF   (*(volatile uint32_t *)(IR_BASE_ADDR + 0x10))  // IR Data Buffer Register
#define IR_IRLCR   (*(volatile uint32_t *)(IR_BASE_ADDR + 0x14))  // IR Line Control Register
#define IR_IRFR    (*(volatile uint32_t *)(IR_BASE_ADDR + 0x18))  // IR Frame Control Register
#define IR_IRPR    (*(volatile uint32_t *)(IR_BASE_ADDR + 0x1C))  // IR Power Control Register
#define IR_IRTCR   (*(volatile uint32_t *)(IR_BASE_ADDR + 0x20))  // IR Timer Control Register
#define IR_IRTDR   (*(volatile uint32_t *)(IR_BASE_ADDR + 0x24))  // IR Transmit Data Register
#define IR_IRRDR   (*(volatile uint32_t *)(IR_BASE_ADDR + 0x28))  // IR Receive Data Register
#define IR_IRFCR   (*(volatile uint32_t *)(IR_BASE_ADDR + 0x2C))  // IR FIFO Control Register
#define IR_IRICR   (*(volatile uint32_t *)(IR_BASE_ADDR + 0x30))  // IR Interrupt Clear Register

#define IR_STATUS_BUSY     (1 << 0)
#define IR_STATUS_DONE     (1 << 1)

#define IR_CONTROL_START   (1 << 0)
#define IR_CONTROL_STOP    (1 << 1)
#define IR_CONTROL_ACK     (1 << 2)

#define TIMEOUT_THRESHOLD 1000000  // Timeout threshold for waiting for status

// Initialize the IRDA interface
int irda_init(void) {
    // Wait for the IRDA to be free
    int timeout = TIMEOUT_THRESHOLD;
    while (IR_SR & IR_STATUS_BUSY) {
        if (--timeout == 0) {
            printf("Error: IRDA is stuck in busy state during init\n");
            return -1;  // Timeout error
        }
    }

    IR_CR = IR_CONTROL_START;  // Start the IRDA module

    // Configure line control register
    IR_IRLCR = 0x03;  // Set line parameters (8N1)
    IR_IRFR = 0x00;   // Configure frame parameters

    printf("IRDA initialized\n");
    return 0;
}

// Send data over IRDA
int irda_send(uint8_t *data, size_t size) {
    // Wait for the IRDA to be free
    int timeout = TIMEOUT_THRESHOLD;
    while (IR_SR & IR_STATUS_BUSY) {
        if (--timeout == 0) {
            printf("Error: IRDA is stuck in busy state during send\n");
            return -1;  // Timeout error
        }
    }

    for (size_t i = 0; i < size; i++) {
        IR_IRTDR = data[i];  // Send byte of data

        // Wait for the data to be transmitted
        timeout = TIMEOUT_THRESHOLD;
        while (!(IR_SR & IR_STATUS_DONE)) {
            if (--timeout == 0) {
                printf("Error: Timeout while waiting for transmission\n");
                return -1;  // Timeout error
            }
        }
    }

    IR_CR |= IR_CONTROL_STOP;  // Stop the IRDA module
    return 0;
}

// Receive data over IRDA
int irda_receive(uint8_t *buffer, size_t size) {
    size_t received = 0;
    int timeout = TIMEOUT_THRESHOLD;

    while (received < size) {
        if (IR_SR & IR_STATUS_DONE) {
            buffer[received++] = (uint8_t)(IR_IRRDR);
            timeout = TIMEOUT_THRESHOLD;
        } else if (--timeout == 0) {
            printf("Error: Timeout while receiving data\n");
            return -1;  // Timeout error
        }
    }

    IR_CR |= IR_CONTROL_STOP;  // Stop the IRDA module
    return 0;
}

// Clear interrupts
void irda_clear_interrupts(void) {
    IR_IRICR = 0xFF;  // Clear all interrupt flags
}
