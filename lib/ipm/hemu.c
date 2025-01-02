#include <stdio.h>
#include <string.h>

// Version information
#define HEMU_VERSION "1.011.2"
#define HEMU_DATE "2025-01-02"

// Supported chipsets
const char *supported_chipsets[] = {
    "UART"  // Basic Universal Asynchronous Receiver-Transmitter
};
const int num_chipsets = sizeof(supported_chipsets) / sizeof(supported_chipsets[0]);

// Function to display supported chipsets
void show_supported_chipsets() {
    printf("Supported chipsets:\n");
    for (int i = 0; i < num_chipsets; i++) {
        printf("  - %s\n", supported_chipsets[i]);
    }
}

// Emulate the UART chipset
void emulate_uart() {
    printf("[HEMU] Emulating UART chipset...\n");
    printf("[HEMU] Initialization complete.\n");
    printf("[HEMU] UART ready for operations.\n");
}

// Parse and execute commands
void hemu_run() {
    char command[256];
    printf("HEMU - Hardware Emulator, %s, %s\n", HEMU_DATE, HEMU_VERSION);
    printf("Welcome to HEMU!\n> ");

    while (fgets(command, sizeof(command), stdin)) {
        // Remove trailing newline
        command[strcspn(command, "\n")] = 0;

        if (strcmp(command, "hemuported") == 0) {
            show_supported_chipsets();
        } else if (strncmp(command, "run ", 4) == 0) {
            const char *chipset = command + 4;
            if (strcmp(chipset, "UART") == 0) {
                emulate_uart();
            } else {
                printf("[HEMU] Error: Unsupported chipset '%s'.\n", chipset);
            }
        } else if (strcmp(command, "exit") == 0) {
            printf("[HEMU] Exiting HEMU. Goodbye!\n");
            break;
        } else {
            printf("[HEMU] Unknown command: '%s'. Try 'hemuported' or 'run <chipset>'.\n", command);
        }

        printf("> ");
    }
}

int hemu_init() {
    hemu_run();
    return 0;
}
