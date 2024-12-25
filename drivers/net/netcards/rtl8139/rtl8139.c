// RTL8139 Network Driver
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "io.h"  // Include the I/O functions (inb, outb, etc.)
#include <stddef.h>
#include <string.h>

// Basic implementation of memcpy
void *memcpy(void *dest, const void *src, size_t n) {
    uint8_t *d = dest;
    const uint8_t *s = src;
    while (n--) {
        *d++ = *s++;
    }
    return dest;
}

// RTL8139 Register base addresses
#define RTL8139_CMD_REG         0x100  // Command register
#define RTL8139_TX_BUF_ADDR     0x200  // TX buffer address
#define RTL8139_RX_BUF_ADDR     0x300  // RX buffer address

// Function to initialize the RTL8139 network card
void init_rtl8139() {
    // Enable the card and start receiving and transmitting packets
    outb(RTL8139_CMD_REG + 0x00, 0x10);  // Command register: enable initialization
    outb(RTL8139_CMD_REG + 0x04, 0x01);  // Enable RX and TX

    // Set the receive buffer address
    outl(RTL8139_RX_BUF_ADDR, 0x4000);  // Set the RX buffer pointer
    // Set the transmit buffer address
    outl(RTL8139_TX_BUF_ADDR, 0x8000);  // Set the TX buffer pointer
}

// Function to send a packet via the network card
void send_packet(uint8_t *packet, size_t size) {
    // Write the packet to the TX buffer
    uint32_t tx_buffer_addr = RTL8139_TX_BUF_ADDR;
    
    // Copy the packet into the buffer
    memcpy((void *)tx_buffer_addr, packet, size);
    
    // Start the transmission
    outb(RTL8139_CMD_REG + 0x10, 0x04);  // Command to start TX
}

// Function to receive a packet from the network card
void receive_packet(uint8_t *buffer, size_t buffer_size) {
    uint32_t rx_buffer_addr = RTL8139_RX_BUF_ADDR;
    
    // Check if there is a packet to receive
    if (inb(RTL8139_CMD_REG + 0x40) & 0x01) {  // Check RX status
        // Read the received packet into the buffer
        memcpy(buffer, (void *)rx_buffer_addr, buffer_size);
    }
}

// Function to enable interrupts for the network card
void enable_interrupts() {
    // Enable interrupts on the card
    outb(RTL8139_CMD_REG + 0x01, 0x08);  // Enable interrupts
}
