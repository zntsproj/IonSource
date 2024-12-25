#ifndef ION_UART_H
#define ION_UART_H

#include <stdint.h>
#include <stdbool.h>

#define DEFAULT_UART_BASE 0x4000C000  // Default base address for UART

// UART Registers (Offset from Base Address)
#define UART_DR    0x00  // Data Register (TX/RX)
#define UART_FR    0x18  // Flag Register (Status Flags)
#define UART_IBRD  0x24  // Integer Baud Rate Divisor
#define UART_FBRD  0x28  // Fractional Baud Rate Divisor
#define UART_LCRH  0x2C  // Line Control Register (data bits, parity, stop bits)
#define UART_CTL   0x30  // Control Register (enable UART)

// Flag Bits in UART_FR Register
#define UART_FR_TXFF (1 << 5)  // Transmit FIFO full flag
#define UART_FR_RXFE (1 << 4)  // Receive FIFO empty flag

// Macros for setting UART base address
#define SET_BASE_ADDR(base) (UART_BASE = (base))

// Global UART base address variable (can be set by the programmer)
static uint32_t UART_BASE = DEFAULT_UART_BASE;

// Function to send a byte via UART
void uart_send(uint8_t c) {
    while (*(volatile uint32_t *)(UART_BASE + UART_FR) & UART_FR_TXFF) {
        // Wait until transmit FIFO is not full
    }
    
    *(volatile uint32_t *)(UART_BASE + UART_DR) = c;
}

// Function to receive a byte via UART
uint8_t uart_receive(void) {
    while (*(volatile uint32_t *)(UART_BASE + UART_FR) & UART_FR_RXFE) {
        // Wait until receive FIFO is not empty
    }
    
    return (uint8_t)(*(volatile uint32_t *)(UART_BASE + UART_DR));
}

// Function to initialize UART with a given baud rate and configuration
void uart_init(uint32_t baud_rate) {
    uint32_t divisor;
    uint32_t integer_part, fractional_part;

    // Calculate the integer and fractional divisors for the baud rate
    // Assuming a 16 MHz system clock (this is often the case in microcontrollers)
    divisor = 16000000 / (16 * baud_rate);  // Divisor formula for baud rate calculation
    integer_part = divisor;  // Integer part of the divisor
    fractional_part = (divisor - integer_part) * 64 + 0.5;  // Fractional part of the divisor

    // Set baud rate (integer and fractional parts)
    *(volatile uint32_t *)(UART_BASE + UART_IBRD) = integer_part;
    *(volatile uint32_t *)(UART_BASE + UART_FBRD) = fractional_part;

    // Configure UART Line Control Register (8 data bits, 1 stop bit, no parity)
    *(volatile uint32_t *)(UART_BASE + UART_LCRH) = (3 << 5);  // 8 bits, no parity, 1 stop bit

    // Enable UART (UART enable, TX and RX enable)
    *(volatile uint32_t *)(UART_BASE + UART_CTL) = (1 << 0) | (1 << 8) | (1 << 9);  // Enable UART, RX, TX
}

#endif // ION_UART_H
