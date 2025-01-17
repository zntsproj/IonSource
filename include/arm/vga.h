#ifndef VGA_H
#define VGA_H

#include "io.h"

// VGA registers for color palette management
#define VGA_PALETTE_INDEX_PORT   0x3C7
#define VGA_PALETTE_DATA_PORT    0x3C9

// Color names (to be mapped to specific RGB values)
#define COLOR_BLACK    0x000000
#define COLOR_BLUE     0x0000FF
#define COLOR_GREEN    0x00FF00
#define COLOR_CYAN     0x00FFFF
#define COLOR_RED      0xFF0000
#define COLOR_MAGENTA  0xFF00FF
#define COLOR_BROWN    0xA52A2A
#define COLOR_LIGHT_GRAY 0xD3D3D3
#define COLOR_DARK_GRAY 0x808080
#define COLOR_LIGHT_BLUE 0xADD8E6
#define COLOR_LIGHT_GREEN 0x90EE90
#define COLOR_LIGHT_CYAN 0xE0FFFF
#define COLOR_LIGHT_RED 0xFF7F7F
#define COLOR_LIGHT_MAGENTA 0xFF77FF
#define COLOR_YELLOW   0xFFFF00
#define COLOR_WHITE    0xFFFFFF

// VGA memory addresses for text mode
#define VGA_TEXT_MODE_ADDR 0xB8000
#define KERNEL_VGA_TEXT_MODE_ADDR 0xA1000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

// Function to set a color in the VGA palette (ARM assembly)
void setpall(const char *color) {
    uint32_t color_value;

    // Map color name to actual RGB value
    if (color == "black") {
        color_value = COLOR_BLACK;
    } else if (color == "blue") {
        color_value = COLOR_BLUE;
    } else if (color == "green") {
        color_value = COLOR_GREEN;
    } else if (color == "cyan") {
        color_value = COLOR_CYAN;
    } else if (color == "red") {
        color_value = COLOR_RED;
    } else if (color == "magenta") {
        color_value = COLOR_MAGENTA;
    } else if (color == "brown") {
        color_value = COLOR_BROWN;
    } else if (color == "light_gray") {
        color_value = COLOR_LIGHT_GRAY;
    } else if (color == "dark_gray") {
        color_value = COLOR_DARK_GRAY;
    } else if (color == "light_blue") {
        color_value = COLOR_LIGHT_BLUE;
    } else if (color == "light_green") {
        color_value = COLOR_LIGHT_GREEN;
    } else if (color == "light_cyan") {
        color_value = COLOR_LIGHT_CYAN;
    } else if (color == "light_red") {
        color_value = COLOR_LIGHT_RED;
    } else if (color == "light_magenta") {
        color_value = COLOR_LIGHT_MAGENTA;
    } else if (color == "yellow") {
        color_value = COLOR_YELLOW;
    } else if (color == "white") {
        color_value = COLOR_WHITE;
    } else {
        return; // Unknown color, no action taken
    }

    // Set the color in the VGA palette (RGB in the format: 8-bit Red, Green, Blue)
    // Writing to VGA palette registers
    __asm__ volatile (
        "mov r0, %0\n"            // Load color index into r0
        "ldr r1, =VGA_PALETTE_INDEX_PORT\n"   // Load palette index port address
        "strb r0, [r1]\n"         // Send color index to VGA palette index register (0x3C7)
        
        "mov r0, %1\n"            // Load Red component into r0
        "ldr r1, =VGA_PALETTE_DATA_PORT\n"   // Load palette data port address
        "strb r0, [r1]\n"         // Send Red component to VGA palette data register (0x3C9)

        "mov r0, %2\n"            // Load Green component into r0
        "strb r0, [r1]\n"         // Send Green component to VGA palette data register (0x3C9)

        "mov r0, %3\n"            // Load Blue component into r0
        "strb r0, [r1]\n"         // Send Blue component to VGA palette data register (0x3C9)
        : 
        : "r"(color_value >> 16), // Red component (most significant byte)
          "r"(color_value >> 8 & 0xFF), // Green component (middle byte)
          "r"(color_value & 0xFF)       // Blue component (least significant byte)
        : "r0", "r1"
    );
}

// Function to print a string to the VGA text buffer (ARM-specific)
void printv(const char *str) {
    volatile char *vga_buffer = (char *)VGA_TEXT_MODE_ADDR;
    int i = 0;

    // Print the string to the VGA text buffer (80x25 text mode)
    while (*str) {
        vga_buffer[i * 2] = *str;          // Character
        vga_buffer[i * 2 + 1] = 0x07;      // Default text color (white on black)
        i++;
        str++;
    }
}

// Function to clear the screen (set all characters to spaces)
void clear_screen() {
    volatile char *vga_buffer = (char *)VGA_TEXT_MODE_ADDR;
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i * 2] = ' ';         // Space character
        vga_buffer[i * 2 + 1] = 0x07;    // Default color (white on black)
    }
}

#endif // VGA_H
