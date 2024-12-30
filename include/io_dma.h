// io_dma.h
#ifndef IO_DMA_H
#define IO_DMA_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

// Maximum number of retries for hardware detection
#define MAX_RETRIES 3

// Function to check if hardware exists at the given address
static bool check_hardware(uintptr_t address) {
    volatile uint32_t *reg = (volatile uint32_t *)address;

    // Try reading from the register
    // If it's a valid hardware register, it shouldn't always be zero
    for (int i = 0; i < MAX_RETRIES; i++) {
        uint32_t val = *reg;
        if (val != 0) {
            return true;
        }
    }

    return false;
}

// Function to write 32-bit value to a register with hardware check 
static void outl_dma(uintptr_t address, uint32_t value) {
    if (check_hardware(address)) {
        // Use assembly to perform a 32-bit write
        __asm__ volatile("movl %0, (%1)" : : "r"(value), "r"(address) : "memory");
    } else {
        printf("No hardware found at register address 0x%lx!\n", address);
    }
}

// Function to read 32-bit value from a register with hardware check
static uint32_t inl_dma(uintptr_t address) {
    if (check_hardware(address)) {
        uint32_t value;
        // Use assembly to perform a 32-bit read
        __asm__ volatile("movl (%1), %0" : "=r"(value) : "r"(address));
        return value;
    } else {
        printf("No hardware found at register address 0x%lx!\n", address);
        return 0;  // Return 0 in case of error
    }
}

// Function to write 16-bit value to a register with hardware check
static void outw_dma(uintptr_t address, uint16_t value) {
    if (check_hardware(address)) {
        // Use assembly to perform a 16-bit write
        __asm__ volatile("movw %0, (%1)" : : "r"(value), "r"(address) : "memory");
    } else {
        printf("No hardware found at register address 0x%lx!\n", address);
    }
}

// Function to read 16-bit value from a register with hardware check
static uint16_t inw_dma(uintptr_t address) {
    if (check_hardware(address)) {
        uint16_t value;
        // Use assembly to perform a 16-bit read
        __asm__ volatile("movw (%1), %0" : "=r"(value) : "r"(address));
        return value;
    } else {
        printf("No hardware found at register address 0x%lx!\n", address);
        return 0;  // Return 0 in case of error
    }
}

// Function to write 8-bit value to a register with hardware check
static void outb_dma(uintptr_t address, uint8_t value) {
    if (check_hardware(address)) {
        // Use assembly to perform an 8-bit write
        __asm__ volatile("movb %0, (%1)" : : "r"(value), "r"(address) : "memory");
    } else {
        printf("No hardware found at register address 0x%lx!\n", address);
    }
}

// Function to read 8-bit value from a register with hardware check
static uint8_t inb_dma(uintptr_t address) {
    if (check_hardware(address)) {
        uint8_t value;
        // Use assembly to perform an 8-bit read
        __asm__ volatile("movb (%1), %0" : "=r"(value) : "r"(address));
        return value;
    } else {
        printf("No hardware found at register address 0x%lx!\n", address);
        return 0;  // Return 0 in case of error
    }
}

#endif // IO_DMA_H
