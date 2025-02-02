#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <i2c/i2c.c>
#include <keyboard/keyboard.c>
#include "rtl8188eu.h"
#include "create_dir.h" // Minimal user api
#include "fsapi.h" // Kernel and user FS api
#include <net/wireless/atheros/hw.c>
#include <hdmi/hdmi.c>
#include <net/wireless/wran/wran.c>
#include <irda/irda.c>
#include "ieee802156.h"
#include "ieee80211.h"
#include <gpio/gpio.c>
#include "panic.c" // Link kernel panic
#include <net/wireless/qcom/qca988x/qca988x.c> // QCOM 988X adapter driver
#include "boot_menu.h"
#include "io_dma.h"
#include "device.h"
#include "uwb/dw1000/dw1000.c"
#include <ipm/hemu.c> // Library path
#include <lic/lic.c> // LICGX C-Compiler

int uwb_option = 0; // 0: off, 1: on for UWB (IONCONFIG)

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

    printf("Enter password (default: root): \n ");
    if (fgets(input, sizeof(input), stdin)) {
        // Remove the newline character, if any
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, DEFAULT_PASSWORD) == 0) {
            printf_log("Password correct! Access granted.\n");
        } else {
            printf_log("Logging into guest...\n");
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
    printf("\033[31mIMPORTANT! Ion kernel may not be fully configured. Configure with: ieeecfg, ionconfig\033[0m\n");
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
    printf("  mkdir <dir_name> - Create a new directory\n");
    printf("  ionconfig - Configure Drivers (on/off)\n");
    printf("  ieeecfg - Configure IEEE Standard\n");
    printf("  fpanic - Force Panic! Only with password\n");
    printf("  setpwd - Set password\n");
}

/**
 * Handles the "echo" command by printing the given message.
 */
void handle_echo(const char *input) {
    const char *message = input + 5;
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

#include "ieee802156.h"

int configure_ieee802156(ieee802156_config_t *config) {
    outl_dma(0x40000000, config->frequency);  // Set the frequency in the device
    outl_dma(0x40000004, config->bandwidth);  // Set the bandwidth in the device
    outl_dma(0x40000008, config->modulation); // Set the modulation type
    outl_dma(0x4000000C, config->power_level); // Set the power level

    return 0;
}

/**
 * Handles the "rtl -c <SSID> <Password>" command.
 * Connects to Wi-Fi using the rtl8188eu driver.
 */
void handle_rtl_connect(const char *input) {
    const char *ssid_start = input + 5;
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
    const char *dir_name = input + 6;
    if (strlen(dir_name) == 0) {
        printf("Invalid syntax! Use: mkdir <dir_name>\n");
        return;
    }

    int result = create_dir(dir_name); // Call the create_dir function to create the directory
    if (result == 0) {
        printf("Directory '%s' created successfully.\n", dir_name); // Success message
    } else {
        printf("Failed to create directory '%s'.\n", dir_name); // Error message
    }
}

/**
 * Handles the "ionconfig" command.
 * Allows users to toggle the drivers on/off.
 */
void handle_ionconfig() {
    int i2c_option = 0; // 0: off, 1: on for I2C
    int gpio_option = 0; // 0: off, 1: on for GPIO
    int qca_option = 0;  // 0: off, 1: on for QCA 988X
    char input;
    int selected_driver = 0; // Selected driver (1: I2C, 2: GPIO, 3: QCA 988X, 4: UWB)

    while (1) {
        system("clear");  // Clear the screen

        // Display available drivers and their current state
        printf("ION CONFIGURATION\n");
        printf("1) [I2C] %s (default: off)\n", i2c_option ? "On" : "Off");
        printf("2) [GPIO] %s (default: off)\n", gpio_option ? "On" : "Off");
        printf("3) [QCA 988X Driver] %s (default: off)\n", qca_option ? "On" : "Off");
        printf("4) [UWB] %s (default: off)\n", uwb_option ? "On" : "Off");  // Добавим UWB
        printf("\nChoose a driver (q to quit): ");
        
        // Read user input for driver selection
        input = getchar();
        getchar(); // Consume the newline character

        if (input == '1') {
            selected_driver = 1;  // I2C driver
        } else if (input == '2') {
            selected_driver = 2;  // GPIO driver
        } else if (input == '3') {
            selected_driver = 3;  // QCA 988X driver
        } else if (input == '4') {
            selected_driver = 4;  // UWB driver
        } else if (input == 'q') {
            break;  // Exit the configuration
        } else {
            printf("Invalid choice! Try again.\n");
            continue;
        }

        while (1) {
            system("clear");  // Clear the screen
            printf("ION CONFIGURATION\n");

            if (selected_driver == 1) {
                printf("[I2C] Current state: %s\n", i2c_option ? "On" : "Off");
            } else if (selected_driver == 2) {
                printf("[GPIO] Current state: %s\n", gpio_option ? "On" : "Off");
            } else if (selected_driver == 3) {
                printf("[QCA 988X Driver] Current state: %s\n", qca_option ? "On" : "Off");
            } else if (selected_driver == 4) {
                printf("[UWB] Current state: %s\n", uwb_option ? "On" : "Off"); 
            }

            printf("\nChoose an option: 1 - On, 2 - Off\n");
            input = getchar();
            getchar(); // Consume the newline character

            if (input == '1') {
                if (selected_driver == 1) {
                    i2c_option = 1;  // Turn on I2C driver
                    create_mknod("i2c", "file=i2c.c");
                } else if (selected_driver == 2) {
                    gpio_option = 1;  // Turn on GPIO driver
                    gpio_init();  // Initialize GPIO pins when driver is turned on
                    create_mknod("gpio", "file=gpio.c");
                } else if (selected_driver == 3) {
                    qca_option = 1;  // Turn on QCA 988X driver
                    qca988x_init();  // Call QCA driver initialization
                    create_mknod("qca988x", "file=qca988x.c");
                } else if (selected_driver == 4) {
                    uwb_option = 1;  // Turn on UWB driver
                    dwuwb_init();  // Initialize UWB driver
                    create_mknod("uwb-dev", "file=uwb.c");
                }
                break;
            } else if (input == '2') {
                if (selected_driver == 1) {
                    i2c_option = 0;  // Turn off I2C driver
                } else if (selected_driver == 2) {
                    gpio_option = 0;  // Turn off GPIO driver
                } else if (selected_driver == 3) {
                    qca_option = 0;  // Turn off QCA 988X driver
                } else if (selected_driver == 4) {
                    uwb_option = 0;  // Turn off UWB driver
                }
                break;
            } else {
                printf("Invalid option! Please select 1 to enable or 2 to disable.\n");
            }
        }
    }
}

void handle_ieeecfg() {
    char choice;
    ieee802156_config_t config;

    // Set default configuration values for IEEE 802.15.6
    config.frequency = IEEE802156_FREQUENCY;
    config.bandwidth = IEEE802156_BANDWIDTH;
    config.modulation = 0;  // Default modulation
    config.power_level = 100;  // Default power level

    system("clear");  // Clear the screen for a fresh display
    printf("\033[1;34m--- Configure IEEE Standard ---\033[0m\n");
    printf("Remember: Even the smallest misstep can lead to a kernel meltdown.\n");
    printf("1) WRAN (802.22)\n");  // Option for WRAN (802.22)
    printf("2) IEEE 802.15.6\n");  // Option for IEEE 802.15.6
    printf("3) IEEE 802.11A\n");   // Option for IEEE 802.11A

    printf("Choose option: ");
    choice = getchar();  // Read user input

    switch (choice) {
        case '1':
            printf("WRAN (802.22) selected\n");
            break;
        case '2':
            printf("IEEE 802.15.6 selected\n");
            // Configure IEEE 802.15.6 with the default settings
            if (configure_ieee802156(&config) == DEVICE_OK) {
                printf("IEEE 802.15.6 configured successfully.\n");
            } else {
                printf("Failed to configure IEEE 802.15.6.\n");
            }
            break;
        case '3':
            printf("IEEE 802.11A selected\n");
            ieee80211_config_t ieee_config = {
                .standard = IEEE80211_11A,
                .is_enabled = 1,
                .initialize = ieee80211_init_11a,
                .shutdown = NULL
            };
            ieee80211_configure(&ieee_config);  // Initialize IEEE 802.11A
            break;
        default:
            printf("Invalid option\n");  // Handle invalid options
            break;
    }
}

#define MAX_PASSWORD_LENGTH 64

// Global variable for storing the current password
static char current_password[MAX_PASSWORD_LENGTH] = DEFAULT_PASSWORD;

// Function to change the password
void set_password(const char* new_password) {
    if (strlen(new_password) < MAX_PASSWORD_LENGTH) {
        strncpy(current_password, new_password, MAX_PASSWORD_LENGTH - 1);
        current_password[MAX_PASSWORD_LENGTH - 1] = '\0'; // Null-terminate
        printf("Password successfully updated!\n");
    } else {
        printf("Error: Password length exceeds the maximum limit of %d characters.\n", MAX_PASSWORD_LENGTH - 1);
    }
}

// Command to handle force_panic with password authentication
void handle_fpanic(const char* input) {
    char entered_password[MAX_PASSWORD_LENGTH];

    printf("Enter password to trigger panic: ");
    if (fgets(entered_password, sizeof(entered_password), stdin)) {
        entered_password[strcspn(entered_password, "\n")] = '\0'; // Remove newline

        if (strcmp(entered_password, current_password) == 0) {
            printf("Password correct! Triggering kernel panic...\n");
            force_panic("Forced panic initiated by user.");
        } else {
            printf("Incorrect password! Panic not triggered.\n");
        }
    } else {
        printf("Error reading input!\n");
    }
}

// Command to set a new password
void handle_setpwd(const char* input) {
    const char* new_password = input + 7; // Skip "setpwd "
    if (strlen(new_password) > 0) {
        set_password(new_password);
    } else {
        printf("Error: Please provide a new password.\n");
    }
}

int main() {
    char input[MAX_INPUT];
    char time_buffer[9]; // Buffer for storing current time
    const char *prompt = "$ "; // Terminal prompt

    boot_menu();
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
            } else if (strcmp(input, "ionconfig") == 0) {
                handle_ionconfig();  // Handle ionconfig
            } else if (strcmp(input, "ieeecfg") == 0) {
                handle_ieeecfg();  // Handle ieeecfg
            } else if (strcmp(input, "fpanic") == 0) {
                            handle_fpanic(input);
            } else if (strncmp(input, "setpwd ", 7) == 0) {
                            handle_setpwd(input);
            } else if (strcmp(input, "ipm get hemu") == 0) {
                printf("Get Hardware Emulator...");
                hemu_init();
            } else if (strcmp(input, "hemu") == 0) {
                hemu_init();  // HEMU
            } else if (strcmp(input, "lic create blank") == 0) {
                lic_create_blank("main.c");
            } else if (strcmp(input, "lic cmp blank") == 0) {
                lic_compile_last_blank();
            } else {
                printf("Unknown command: %s\n", input); // Handle unknown commands
            }
        }
    }

    return 0;
}

