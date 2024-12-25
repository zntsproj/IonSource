#include "panic.h"

// Panic handler definition
int force_panic(const char* message) {
    panic("SYSTEM HALTED DUE TO CRITICAL ERROR!!!");
    
    // Print detailed information about the panic
    printf("ERROR: %s\n", message);
    printf("This is a kernel panic, and the system cannot continue.\n");

    // Additional debug information
    printf("Please check the kernel logs for more details.\n");
    printf("Error occurred in function: %s\n", __func__);
    printf("In file: %s\n", __FILE__);
    printf("At line: %d\n", __LINE__);
    
    // System halted indefinitely
    printf("System halt initiated. Kernel will not continue execution.\n");
    asm volatile ("hlt");


    return 0;
}
