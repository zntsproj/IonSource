#ifndef IONSTDIO_H
#define IONSTDIO_H

#include "types.h"

// Define log levels
#define KERN_INFO  0x01
#define KERN_ERROR 0x02
#define KERN_FATAL 0x03
#define KERN_WARN  0x04

// Inline assembly function for volatile operations
static inline void asm_printk(uint8_t log_level, const char* message) {
    __asm__ volatile (
        "mov r0, %0\n"       // Move log_level into r0
        "mov r1, %1\n"       // Move message pointer into r1
        "svc 0x00\n"         // Trigger software interrupt for logging
        : // No output operands
        : "r"(log_level), "r"(message)
        : "r0", "r1", "memory"
    );
}

// Macro to simplify usage
#define printk(level, fmt, ...) \
    do { \
        char buffer[256]; \
        snprintf(buffer, sizeof(buffer), fmt, ##__VA_ARGS__); \
        asm_printk(level, buffer); \
    } while (0)

#endif // IONSTDIO_H
