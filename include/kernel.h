// kernel.h
// Global fixes: on 2025-01-18, compile error (fix)
#ifndef ION_KERNEL_H
#define ION_KERNEL_H

#include "vga.h"
#include <stdarg.h> // For handling variable argument lists
#include <stddef.h> // For size_t type
#include <stdint.h> // For uint32_t and other standard types

// Forward declaration of itoa
void itoa(int value, char* str, int base);

// Function to print formatted messages to the screen, similar to printk
void printk(const char* format, ...) {
    va_list args;
    va_start(args, format);

    const char* ptr = format;

    while (*ptr != '\0') {
        if (*ptr == '%' && *(ptr + 1) == 's') {
            // Handle string format (%s)
            char* str = va_arg(args, char*);
            printv(str);
            ptr += 2;
        } else if (*ptr == '%' && *(ptr + 1) == 'd') {
            // Handle integer format (%d)
            int num = va_arg(args, int);
            char buffer[20];
            itoa(num, buffer, 10);  // Convert number to string
            printv(buffer);
            ptr += 2;
        } else {
            // Print a single character if not a format specifier
            volatile char *vga_buffer = (char *)VGA_TEXT_MODE_ADDR;
            int pos = 0; // Replace this with logic to manage cursor position
            vga_buffer[pos * 2] = *ptr;
            vga_buffer[pos * 2 + 1] = 0x07;
            ptr++;
        }
    }

    va_end(args);
}

// Function to convert an integer to a string (itoa)
void itoa(int value, char* str, int base) {
    int i = 0;
    int sign = value;

    if (sign < 0) {
        value = -value;
    }

    // Convert integer to string
    do {
        str[i++] = value % base + '0';
    } while ((value /= base) > 0);

    if (sign < 0) {
        str[i++] = '-';
    }

    str[i] = '\0';

    // Reverse the string
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

#endif // ION_KERNEL_H
