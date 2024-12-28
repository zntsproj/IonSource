#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <i2c/i2c.c>
#include <keyboard/keyboard.c>
#include "rtl8188eu.h"
#include "create_dir.h" // Include the header for directory creation
#include <net/wireless/atheros/hw.c>
#include <hdmi/hdmi.c>

// Default password
#define DEFAULT_PASSWORD "root"
#define MAX_INPUT 256

void printf_log(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    printf("[LOG] "); // Prefix for log messages
    vprintf(fmt, args);

    va_end(args);
}

void interactive_text() {
    char input[256]; // Buffer to store user input

    printf("Enter password (default: root): ");
    if (fgets(input, sizeof(input), stdin)) {
        // Remove the newline character, if any
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, DEFAULT_PASSWORD) == 0) {
            printf_log("Password correct! Access granted.\n");
        } else {
            printf_log("Incorrect password! Logging into guest...\n");
        }
    } else {
        printf_log("Error reading input!\n");
    }
}

void hdmi_irq_handler(void *data) {
    printf("HDMI interrupt handled! Data: %p\n", data);
}

void print_welcome() {
    printf("\033[33mIon Terminal 1.05 Stable - type help for basic commands\033[0m\n");
    printf("\033[1;35mSupport Ion on Open Collective: https://opencollective.com/ion-kernel\033[0m\n");
}

void get_current_time(char *buffer, size_t size) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(buffer, size, "%H:%M:%S", tm_info);
}

/**
 * Handles the "help" command by displaying available commands.
 */
void handle_help() {
    printf("Basic commands:\n");
    printf("  help - Show this help message\n");
    printf("  echo <message> - Print the given message\n");
    printf("  exit - Exit the terminal\n");
    printf("  sysinfo - shows the system information\n");
    printf("  wlsctl --help - Show supported Wi-Fi cards\n");
    printf("  rtl -c <SSID> <Password> - Connect to Wi-Fi using rtl8188eu\n");
    printf("  mkdir <dir_name> - Create a new directory\n"); // Added the new command description
}

/**
 * Handles the "echo" command by printing the given message.
 */
void handle_echo(const char *input) {
    const char *message = input + 5; // Skip the "echo " part
    printf("%s\n", message);
}

void handle_shutdown() {
    printf("Shutting down the system...\n");

    // Assembly code to halt the system
    asm volatile (
        "mov $0xF4, %al\n"    // BIOS interrupt for power off
        "outb %al, $0x64\n"   // Send the shutdown signal to the system
        "hlt"                 // Halt the CPU
    );

    // Infinite loop in case hlt does not stop the system
    while (1);
}

void sysinfo_command() {
    #ifdef __x86_64__
        printf("[SYSINFO] Architecture: x86_64\n");
    #elif __i386__
        printf("[SYSINFO] Architecture: x86\n");
    #elif __arm__
        printf("[SYSINFO] Architecture: ARM\n");
    #else
        printf("[SYSINFO] Unknown Architecture...\n");
    #endif
}

/**
 * Handles the "wlsctl --help" command.
 * Shows supported Wi-Fi cards (currently only rtl8188eu).
 */
void handle_wlsctl_help() {
    printf("Supported Wi-Fi cards:\n");
    printf("  rtl8188eu - Realtek RTL8188EU USB Wi-Fi adapter\n");
}

/**
 * Handles the "rtl -c <SSID> <Password>" command.
 * Connects to Wi-Fi using the rtl8188eu driver.
 */
void handle_rtl_connect(const char *input) {
    const char *ssid_start = input + 5; // Skip "rtl -c " part
    char ssid[256];
    char password[256];

    // Parse SSID and password
    if (sscanf(ssid_start, "%s %s", ssid, password) == 2) {
        int result = rtl8188eu_connect(ssid, password, 100000);
        if (result == 0) {
            printf("Successfully connected to Wi-Fi network %s\n", ssid);
        } else {
            printf("Failed to connect to Wi-Fi network %s\n", ssid);
        }
    } else {
        printf("Invalid syntax! Use: rtl -c <SSID> <Password>\n");
    }
}

/**
 * Handles the "mkdir" command by creating a directory.
 */
void handle_mkdir(const char *input) {
    const char *dir_name = input + 6; // Skip "mkdir " part
    if (strlen(dir_name) == 0) {
        printf("Invalid syntax! Use: mkdir <dir_name>\n");
        return;
    }

    int result = create_dir(dir_name); // Call the create_dir funfction to create the directory
    if (result == 0) {
        printf("Directory '%s' created successfully.\n", dir_name); // Success message
    } else {
        printf("Failed to create directory '%s'.\n", dir_name); // Error message
    }
}

int main() {
    char input[MAX_INPUT];
    char time_buffer[9]; // Buffer for storing current time
    const char *prompt = "$ "; // Terminal prompt

    printf_log("Kernel loaded at 0x10000\n");
    printf_log("Kernel booting...\n");
    printf_log("Welcome to ION Kernel!\n");
    printf_log("Loaded i2c driver.\n");
    printf_log("Loaded keyboard driver.\n");
    printf("[WIRELESS] Loaded Wi-Fi driver (rtl8188eu).\n");
    printf("[WIRELESS] Loaded WI-FI Atheros driver.\n");
    printf("[HDMI] Driver loaded...\n");
    interactive_text();

    print_welcome();
    while (1) {
        // Get the current time
        get_current_time(time_buffer, sizeof(time_buffer));

        // Display the terminal interface with the current time
        printf("[%s] %s", time_buffer, prompt);
        fflush(stdout);

        // Wait for user input
        if (fgets(input, sizeof(input), stdin) != NULL) {
            // Remove the newline character from the input
            size_t len = strlen(input);
            if (len > 0 && input[len - 1] == '\n') {
                input[len - 1] = '\0';
            }

            // Process the user command
            if (strcmp(input, "help") == 0) {
                handle_help();
            } else if (strncmp(input, "echo ", 5) == 0) {
                handle_echo(input);
            } else if (strcmp(input, "shutdown") == 0) {
                handle_shutdown();  // Call shutdown function
            } else if (strcmp(input, "sysinfo") == 0) {
                sysinfo_command();  // Show system information
            } else if (strcmp(input, "exit") == 0) {
                printf("Exiting terminal...\n");
                break;
            } else if (strcmp(input, "wlsctl --help") == 0) {
                handle_wlsctl_help();  // Show supported Wi-Fi cards
            } else if (strncmp(input, "rtl -c ", 7) == 0) {
                handle_rtl_connect(input);  // Connect to Wi-Fi
            } else if (strncmp(input, "mkdir ", 6) == 0) {
                handle_mkdir(input);  // Create directory
            } else {
                printf("Unknown command: %s\n", input); // Handle unknown commands
            }
        }
    }

    return 0;
}
