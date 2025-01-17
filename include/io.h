#ifndef IO_H
#define IO_H

// Custom kernel include note
#warning "This is a kernel include! Only use in kernel-space!"

#include <stdint.h>
#include <stddef.h>

// Function to read a byte from an I/O port
static inline uint8_t inb(uint16_t port) {
    uint8_t result;
    __asm__ volatile("inb %1, %0" : "=a"(result) : "d"(port));
    return result;
}

// Function to write a byte to an I/O port
static inline void outb(uint16_t port, uint8_t value) {
    __asm__ volatile("outb %0, %1" : : "a"(value), "d"(port));
}

// Function to read a word (2 bytes) from an I/O port
static inline uint16_t inw(uint16_t port) {
    uint16_t result;
    __asm__ volatile("inw %1, %0" : "=a"(result) : "d"(port));
    return result;
}

// Function to write a word (2 bytes) to an I/O port
static inline void outw(uint16_t port, uint16_t value) {
    __asm__ volatile("outw %0, %1" : : "a"(value), "d"(port));
}

// Function to read a double word (4 bytes) from an I/O port
static inline uint32_t inl(uint16_t port) {
    uint32_t result;
    __asm__ volatile("inl %1, %0" : "=a"(result) : "d"(port));
    return result;
}

// Function to write a double word (4 bytes) to an I/O port
static inline void outl(uint16_t port, uint32_t value) {
    __asm__ volatile("outl %0, %1" : : "a"(value), "d"(port));
}

// Function to read a quad word (8 bytes) from an I/O port
static inline uint64_t inq(uint16_t port) {
    uint64_t result;
    __asm__ volatile("inq %1, %0" : "=A"(result) : "d"(port));
    return result;
}

// Function to write a quad word (8 bytes) to an I/O port
static inline void outq(uint16_t port, uint64_t value) {
    __asm__ volatile("outq %0, %1" : : "A"(value), "d"(port));
}

// Function to read a byte from a specific port and store in a buffer
static inline void inb_buffer(uint16_t port, uint8_t *buffer, size_t size) {
    for (size_t i = 0; i < size; i++) {
        buffer[i] = inb(port);  // Read each byte
    }
}

// Function to write a byte buffer to a specific I/O port
static inline void outb_buffer(uint16_t port, const uint8_t *buffer, size_t size) {
    for (size_t i = 0; i < size; i++) {
        outb(port, buffer[i]);  // Write each byte
    }
}

// Function to wait for the I/O port to be ready for communication (busy-wait)
static inline void io_wait(void) {
    __asm__ volatile("jmp 1f\n1:");
}

// Function to check if the I/O port is ready (polling)
static inline int io_ready(uint16_t port) {
    return (inb(port) & 0x01);  // Check if the status register indicates readiness
}

#endif // IO_H
