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

int irda_init(void) {
    while (IR_SR & IR_STATUS_BUSY);

    IR_CR = IR_CONTROL_START;

    IR_IRLCR = 0x03;
    IR_IRFR = 0x00;

    printf("IRDA initialized\n");
    return 0;
}

int irda_send(uint8_t *data, size_t size) {
    while (IR_SR & IR_STATUS_BUSY);

    for (size_t i = 0; i < size; i++) {
        IR_IRTDR = data[i];

        while (!(IR_SR & IR_STATUS_DONE));
    }

    IR_CR |= IR_CONTROL_STOP;
    return 0;
}

int irda_receive(uint8_t *buffer, size_t size) {
    size_t received = 0;
    while (received < size) {
        if (IR_SR & IR_STATUS_DONE) {
            buffer[received++] = (uint8_t)(IR_IRRDR);
        }
    }

    IR_CR |= IR_CONTROL_STOP;
    return 0;
}

void irda_clear_interrupts(void) {
    IR_IRICR = 0xFF;
}
