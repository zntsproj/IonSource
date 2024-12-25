#ifndef TTY_H
#define TTY_H

#include "vga.h"  // Include VGA for screen output
#include "io.h"   // Include for I/O operations like inb

#define TTY_MAX_INPUT_LENGTH  128  // Maximum length of input buffer

// Structure to store the terminal state
typedef struct {
    char input_buffer[TTY_MAX_INPUT_LENGTH];  // Buffer for user input
    int input_index;                          // Index for the current input position
} tty_state;

tty_state terminal_state;  // Declare a global variable for terminal state

// Function declarations
void tty_execute_command(const char* command);  // Declare the function before its usage
void tty_say(const char* text);  // Declare the function for "say" command

// Function to initialize the terminal
void tty_init() {
    vga_init();  // Initialize VGA display
    vga_print_string("Welcome to Ion Kernel!\n", VGA_COLOR_WHITE);
    vga_print_string("Ion Terminal. ver. 2024.12.22\n", VGA_COLOR_WHITE);
    vga_print_string("Login (Default: root): ", VGA_COLOR_WHITE);

    terminal_state.input_index = 0;  // Initialize the input index to 0
}

// Function to handle keyboard input
void tty_handle_input() {
    unsigned char c = inb(0x60);  // Read a character from the keyboard

    // Process the character
    if (c == '\n') {  // If the input is a newline, process the command
        terminal_state.input_buffer[terminal_state.input_index] = '\0';  // Null-terminate the string
        vga_print_string("\n", VGA_COLOR_WHITE);  // Move to the next line
        tty_execute_command(terminal_state.input_buffer);  // Execute the command
        terminal_state.input_index = 0;  // Reset input index for the next command
        vga_print_string("ROOT>_", VGA_COLOR_WHITE);  // Print the prompt
    } else if (c == '\b' && terminal_state.input_index > 0) {  // Handle backspace
        terminal_state.input_index--;
        vga_print_char(' ', VGA_COLOR_BLACK);  // Clear the character from the screen
        vga_print_char('\b', VGA_COLOR_BLACK);  // Move the cursor back
    } else if (terminal_state.input_index < TTY_MAX_INPUT_LENGTH - 1) {
        terminal_state.input_buffer[terminal_state.input_index] = c;  // Add the character to the buffer
        terminal_state.input_index++;  // Move the input index forward
        vga_print_char(c, VGA_COLOR_WHITE);  // Display the character on the screen
    }
}

// Function to execute commands
void tty_execute_command(const char* command) {
    if (command[0] == 's' && command[1] == 'a' && command[2] == 'y') {  // Check if the command is "say"
        vga_print_string(command + 4, VGA_COLOR_GREEN);  // Print the text after the "say" command
    }
}

// Function to print a message after the "say" command
void tty_say(const char* text) {
    vga_print_string(text, VGA_COLOR_GREEN);  // Print the text on the screen
}

#endif // TTY_H
