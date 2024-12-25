/* 
 * UART Driver for simple serial communication
 * Copyright (C) 2024 [Your Name] <znts543@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdint.h>

#define UART0_BASE 0x4000C000     // Base address for UART0
#define UART0_DR   *(volatile uint32_t *)(UART0_BASE + 0x00)  // Data register
#define UART0_FR   *(volatile uint32_t *)(UART0_BASE + 0x18)  // Flag register

// Function to send a byte via UART
void uart_send(uint8_t c) {
    // Wait until the UART transmit FIFO is not full (TX FIFO empty flag)
    while (UART0_FR & (1 << 5)) {
        // Busy-wait until transmit FIFO is ready
    }
    
    UART0_DR = c;  // Write the byte to the data register to transmit it
}

// Function to receive a byte via UART
uint8_t uart_receive(void) {
    // Wait until the UART receive FIFO has data (RX FIFO non-empty flag)
    while (UART0_FR & (1 << 4)) {
        // Busy-wait until receive FIFO has data
    }
    
    return (uint8_t)(UART0_DR);  // Read the received byte from the data register
}

int main(void) {
    // Test the UART driver by sending and receiving data

    uart_send('H');  // Send character 'H'
    uart_send('e');  // Send character 'e'
    uart_send('l');  // Send character 'l'
    uart_send('l');  // Send character 'l'
    uart_send('o');  // Send character 'o'

    // Wait for a character to be received and echo it
    uint8_t received_byte = uart_receive();  // Receive a byte
    uart_send(received_byte);  // Echo received byte back

    return 0;
}
