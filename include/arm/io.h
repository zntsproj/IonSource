#ifndef IO_H
#define IO_H

// Kernel include note
#warning "This is a kernel include! Only use in kernel-space!"

#include <stdint.h>
#include <stddef.h>

// Function to read a byte from a memory-mapped I/O address
static inline uint8_t mmio_read8(uintptr_t addr) {
    uint8_t result;
    __asm__ volatile("ldrb %0, [%1]" : "=r"(result) : "r"(addr));
    return result;
}

// Function to write a byte to a memory-mapped I/O address
static inline void mmio_write8(uintptr_t addr, uint8_t value) {
    __asm__ volatile("strb %0, [%1]" : : "r"(value), "r"(addr));
}

// Function to read a 16-bit word from a memory-mapped I/O address
static inline uint16_t mmio_read16(uintptr_t addr) {
    uint16_t result;
    __asm__ volatile("ldrh %0, [%1]" : "=r"(result) : "r"(addr));
    return result;
}

// Function to write a 16-bit word to a memory-mapped I/O address
static inline void mmio_write16(uintptr_t addr, uint16_t value) {
    __asm__ volatile("strh %0, [%1]" : : "r"(value), "r"(addr));
}

// Function to read a 32-bit word from a memory-mapped I/O address
static inline uint32_t mmio_read32(uintptr_t addr) {
    uint32_t result;
    __asm__ volatile("ldr %0, [%1]" : "=r"(result) : "r"(addr));
    return result;
}

// Function to write a 32-bit word to a memory-mapped I/O address
static inline void mmio_write32(uintptr_t addr, uint32_t value) {
    __asm__ volatile("str %0, [%1]" : : "r"(value), "r"(addr));
}

// Function to read a 64-bit word from a memory-mapped I/O address
static inline uint64_t mmio_read64(uintptr_t addr) {
    uint64_t result;
    __asm__ volatile("ldr %0, [%1]" : "=r"(result) : "r"(addr));
    return result;
}

// Function to write a 64-bit word to a memory-mapped I/O address
static inline void mmio_write64(uintptr_t addr, uint64_t value) {
    __asm__ volatile("str %0, [%1]" : : "r"(value), "r"(addr));
}

// Function to wait for a memory-mapped register to become ready
static inline void mmio_wait_ready(uintptr_t addr, uint32_t mask) {
    while (!(mmio_read32(addr) & mask)) {
        __asm__ volatile("nop"); // Wait (No Operation)
    }
}

// Function to poll a memory-mapped register until a specific value is set
static inline void mmio_poll(uintptr_t addr, uint32_t value) {
    while (mmio_read32(addr) != value) {
        __asm__ volatile("nop"); // Wait (No Operation)
    }
}

#endif // IO_H
