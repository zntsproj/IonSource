#ifndef IONSTDIO_H
#define IONSTDIO_H

#include "io.h"

// Function prototypes
void kputc(char c);
void kputs(const char *str);
void kprintf(const char *fmt, ...);

// Write a single character using low-level I/O
void kputc(char c) {
    __asm__ volatile (
        "mov r0, %0\n"   // Move character to register r0
        "bl outb\n"      // Call our low-level outb function
        :                // No output
        : "r"(c)         // Input: character to print
        : "r0"           // Clobbered: r0
    );
}

// Write a null-terminated string
void kputs(const char *str) {
    while (*str) {
        kputc(*str++);
    }
}

void kprintf(const char *fmt, ...) {
    __asm__ volatile (
        "push {r0-r3, lr}       \n"  // Save registers (r0-r3, lr)

        "mov r1, r0            \n"  // r1 = fmt (format string argument)
        "add r2, sp, #16       \n"  // r2 points to the arguments (...), skipping saved registers

        "1:                    \n"  // Loop label
        "ldrb r3, [r1], #1     \n"  // r3 = *fmt++, load the next character from the format string
        "cmp r3, #0            \n"  // Check if it's the end of the string
        "beq 2f                \n"  // If end of string, exit the loop

        "cmp r3, #'%'          \n"  // Check if the character is '%'
        "bne 3f                \n"  // If not '%', print the character

        "ldrb r3, [r1], #1     \n"  // Load the next character after '%'
        "cmp r3, #'c'          \n"  // Check if it's the '%c' format
        "bne 4f                \n"  // If not '%c', go to handling other formats

        // Handling '%c' format
        "ldr r4, [r2], #4      \n"  // Load the next argument (character)
        "bl kputc              \n"  // Call kputc to print the character
        "b 1b                  \n"  // Continue to the next character

        "3:                    \n"  // Print the character (non-format)
        "mov r4, r3            \n"  // r4 = character to print
        "bl kputc              \n"  // Call kputc to print the character
        "b 1b                  \n"  // Continue to the next character

        "4:                    \n"  
        "b 1b                  \n"

        "2:                    \n"  // End of string
        "pop {r0-r3, lr}       \n"  // Restore registers
        "bx lr                 \n"  // Return from function
    );
}


#endif // IONSTDIO_H
