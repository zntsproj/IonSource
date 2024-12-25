#ifndef PANIC_H
#define PANIC_H

#include <stdio.h>

// Panic handler
void panic(const char* message) {
    // Print panic header
    printf("--- KERNEL PANIC!!! ---\n");
    printf("ERROR: %s\n", message);

    // Halt the system indefinitely (for demonstration purposes)
    while (1);
}

#endif // PANIC_H
