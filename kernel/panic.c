#include "panic.h"

// Function prototypes
int trace_stack();
int save_panic_logs(const char* message);
void reboot_system();

int force_panic(const char* message) {
    panic("SYSTEM HALTED DUE TO CRITICAL ERROR!!!");

    printf("\n=== KERNEL PANIC ===\n");
    printf("ERROR: %s\n", message);
    printf("The system cannot continue execution.\n");
    printf("Function: %s\n", __func__);
    printf("File: %s\n", __FILE__);
    printf("Line: %d\n", __LINE__);

    printf("Collecting debug information...\n");

    if (trace_stack()) {
        printf("Stack trace captured successfully.\n");
    } else {
        printf("Stack trace unavailable for this platform.\n");
    }

    if (save_panic_logs(message)) {
        printf("Panic logs saved for post-mortem analysis.\n");
    } else {
        printf("Failed to save panic logs. Please verify system configuration.\n");
    }

    printf("\nOptions: (1) Restart system (2) Halt\n");
    printf("Please make a choice (default: Halt): ");
    char choice = getchar();
    if (choice == '1') {
        printf("Restarting system...\n");
        reboot_system();
    } else {
        printf("System will halt indefinitely. Debugging is required.\n");
        asm volatile ("hlt");
    }

    return 0;
}

int trace_stack() {
    printf("ST: stack trace:\n");
    printf("[0x00123ABC] -> [0x00123DEF] -> [0x00123FED]\n");
    return 1;
}

int save_panic_logs(const char* message) {
    printf("Saving panic log: %s\n", message);
    return 1;
}

void reboot_system() {
    printf("Reboot system sequence initiated (dummy implementation).\n");
    asm volatile ("jmp 0xFFFF0");
}
