#ifndef VGA_H
#define VGA_H

#include "io.h"
#include <string.h> // FIX on: 2025-01-18: add strcmp

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
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

// Function to set a color in the VGA palette
void setpall(const char *color) {
    uint32_t color_value;

    if (strcmp(color, "black") == 0) {
        color_value = COLOR_BLACK;
    } else if (strcmp(color, "blue") == 0) {
        color_value = COLOR_BLUE;
    } else if (strcmp(color, "green") == 0) {
        color_value = COLOR_GREEN;
    } else if (strcmp(color, "cyan") == 0) {
        color_value = COLOR_CYAN;
    } else if (strcmp(color, "red") == 0) {
        color_value = COLOR_RED;
    } else if (strcmp(color, "magenta") == 0) {
        color_value = COLOR_MAGENTA;
    } else if (strcmp(color, "brown") == 0) {
        color_value = COLOR_BROWN;
    } else if (strcmp(color, "light_gray") == 0) {
        color_value = COLOR_LIGHT_GRAY;
    } else if (strcmp(color, "dark_gray") == 0) {
        color_value = COLOR_DARK_GRAY;
    } else if (strcmp(color, "light_blue") == 0) {
        color_value = COLOR_LIGHT_BLUE;
    } else if (strcmp(color, "light_green") == 0) {
        color_value = COLOR_LIGHT_GREEN;
    } else if (strcmp(color, "light_cyan") == 0) {
        color_value = COLOR_LIGHT_CYAN;
    } else if (strcmp(color, "light_red") == 0) {
        color_value = COLOR_LIGHT_RED;
    } else if (strcmp(color, "light_magenta") == 0) {
        color_value = COLOR_LIGHT_MAGENTA;
    } else if (strcmp(color, "yellow") == 0) {
        color_value = COLOR_YELLOW;
    } else if (strcmp(color, "white") == 0) {
        color_value = COLOR_WHITE;
    } else {
        return; // Unknown color, no action taken
    }

    // Set the color in the VGA palette using I/O operations
    outb(VGA_PALETTE_INDEX_PORT, (color_value >> 16) & 0xFF);  // Send the index (R)
    outb(VGA_PALETTE_DATA_PORT, (color_value >> 8) & 0xFF);    // Send the data (G)
    outb(VGA_PALETTE_DATA_PORT, color_value & 0xFF);           // Send the data (B)
    outb(VGA_PALETTE_DATA_PORT, color_value & 0xFF);           // Repeat the data (for internal register setup)
}

// Function to print a string to the VGA text buffer
void printv(const char *str) {
    volatile char *vga_buffer = (char *)VGA_TEXT_MODE_ADDR;
    int i = 0;

    while (*str && i < VGA_WIDTH * VGA_HEIGHT) {
        vga_buffer[i * 2] = *str;      // Character
        vga_buffer[i * 2 + 1] = 0x07; // Default text color (white on black)
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
