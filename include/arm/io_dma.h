// io_dma_arm.h
#ifndef IO_DMA_ARM_H
#define IO_DMA_ARM_H

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

// Function to write 32-bit value to a register with hardware check using ARM assembly
static void outl_dma_arm(uintptr_t address, uint32_t value) {
    if (check_hardware(address)) {
        // Use ARM assembly to perform a 32-bit write
        __asm__ volatile("str %0, [%1]" : : "r"(value), "r"(address) : "memory");
    } else {
        printf("No hardware found at register address 0x%lx!\n", address);
    }
}

// Function to read 32-bit value from a register with hardware check using ARM assembly
static uint32_t inl_dma_arm(uintptr_t address) {
    if (check_hardware(address)) {
        uint32_t value;
        // Use ARM assembly to perform a 32-bit read
        __asm__ volatile("ldr %0, [%1]" : "=r"(value) : "r"(address));
        return value;
    } else {
        printf("No hardware found at register address 0x%lx!\n", address);
        return 0;  // Return 0 in case of error
    }
}

// Function to write 16-bit value to a register with hardware check using ARM assembly
static void outw_dma_arm(uintptr_t address, uint16_t value) {
    if (check_hardware(address)) {
        // Use ARM assembly to perform a 16-bit write
        __asm__ volatile("strh %0, [%1]" : : "r"(value), "r"(address) : "memory");
    } else {
        printf("No hardware found at register address 0x%lx!\n", address);
    }
}

// Function to read 16-bit value from a register with hardware check using ARM assembly
static uint16_t inw_dma_arm(uintptr_t address) {
    if (check_hardware(address)) {
        uint16_t value;
        // Use ARM assembly to perform a 16-bit read
        __asm__ volatile("ldrh %0, [%1]" : "=r"(value) : "r"(address));
        return value;
    } else {
        printf("No hardware found at register address 0x%lx!\n", address);
        return 0;  // Return 0 in case of error
    }
}

// Function to write 8-bit value to a register with hardware check using ARM assembly
static void outb_dma_arm(uintptr_t address, uint8_t value) {
    if (check_hardware(address)) {
        // Use ARM assembly to perform an 8-bit write
        __asm__ volatile("strb %0, [%1]" : : "r"(value), "r"(address) : "memory");
    } else {
        printf("No hardware found at register address 0x%lx!\n", address);
    }
}

// Function to read 8-bit value from a register with hardware check using ARM assembly
static uint8_t inb_dma_arm(uintptr_t address) {
    if (check_hardware(address)) {
        uint8_t value;
        // Use ARM assembly to perform an 8-bit read
        __asm__ volatile("ldrb %0, [%1]" : "=r"(value) : "r"(address));
        return value;
    } else {
        printf("No hardware found at register address 0x%lx!\n", address);
        return 0;  // Return 0 in case of error
    }
}

#endif // IO_DMA_ARM_H
