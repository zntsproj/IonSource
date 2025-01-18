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
        "movb %0, %%al\n"      // Move character to AL register (low byte of EAX)
        "outb %%al, $0x3F8\n"  // Send the character to the I/O port (0x3F8 for COM1)
        :                       // No output
        : "r"(c)                // Input: character to print
        : "%al"                 // Clobbered register: AL
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
        "push %ebx\n"            // Save EBX
        "push %ecx\n"            // Save ECX
        "push %edx\n"            // Save EDX
        "movl %0, %%eax\n"       // Move fmt (format string) into EAX
        "movl 8(%%ebp), %%ecx\n" // Address of the first argument
        "1:\n"
        "movb (%%eax), %%al\n"   // Load the next byte of the format string into AL
        "inc %%eax\n"            // Increment the format string pointer
        "cmpb $0, %%al\n"        // Check for null terminator
        "je 2f\n"                // End of string

        "cmpb $37, %%al\n"       // Check if the character is '%'
        "jne 3f\n"               // If not '%', jump to print the character

        "movb (%%eax), %%al\n"   // Load the next character after '%' (e.g., 'c' for %c)
        "inc %%eax\n"            // Increment pointer to the next character
        "cmpb $'c', %%al\n"      // Check if it's '%c'
        "jne 4f\n"               // If not '%c', jump to handle other formats

        // Handle '%c' format
        "movl %%ecx, %%edx\n"    // Load the argument for '%c' from the stack
        "movb (%%edx), %%al\n"   // Load the character into AL
        "call kputc\n"           // Call kputc to print the character
        "addl $4, %%ecx\n"       // Move the argument pointer forward (4 bytes per argument)
        "jmp 1b\n"               // Jump back to loop

        "3:\n"                   // Print the character (non-format)
        "movb (%%eax), %%al\n"   // Load the character into AL
        "call kputc\n"           // Call kputc to print the character
        "jmp 1b\n"               // Jump back to loop

        "4:\n"
        "jmp 1b\n"

        "2:\n"                   // End of string
        "pop %edx\n"             // Restore EDX
        "pop %ecx\n"             // Restore ECX
        "pop %ebx\n"             // Restore EBX
        :
        : "r"(fmt)               // Input: format string
        : "%eax", "%ebx", "%ecx", "%edx" // Clobbered registers
    );
}

#endif // IONSTDIO_H
