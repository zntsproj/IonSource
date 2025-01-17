#ifndef MEM_H
#define MEM_H

#include <stddef.h>
#include <stdint.h>
#include "ionstdio.h"

// Function prototypes
void* alloc(const char *size_str);

// Implementation of alloc function
void* alloc(const char *size_str) {
    size_t size = 0;

    // Convert size string to integer
    while (*size_str) {
        if (*size_str >= '0' && *size_str <= '9') {
            size = size * 10 + (*size_str - '0');
        } else {
            return NULL; // Invalid input
        }
        size_str++;
    }

    // Allocate memory using a simple kernel-level mechanism
    void* ptr = NULL;

    __asm__ volatile (
        "movl %1, %%ecx    \n" // Size to ECX
        "int $0x80         \n" // Trigger syscall for allocation
        "movl %%eax, %0    \n" // Pointer to EAX
        : "=r"(ptr)          // Output: allocated pointer
        : "r"(size)          // Input: size in bytes
        : "%ecx", "%eax"   // Clobbered registers
    );

    // Print allocation message in green
    if (ptr) {
        kputs("\033[32m"); // Set text color to green
        kprintf("%u bytes allocated!\n", (unsigned int)size);
        kputs("\033[0m"); // Reset text color
    }

    return ptr;
}

#endif // MEM_H
