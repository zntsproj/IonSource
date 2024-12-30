#ifndef BOOT_MENU_H
#define BOOT_MENU_H

#include <stdio.h>
#include <stdlib.h>

void boot_menu() {
    // Clear the screen for a fresh boot menu display
    system("clear");

    // Display ASCII art and menu options
    printf("\033[1;34m"
           " ___  ________  ________      \n"
           "|\\  \\|\\   __  \\|\\   ___  \\    \n"
           "\\ \\  \\ \\  \\|\\  \\ \\  \\\\ \\  \\   \n"
           " \\ \\  \\ \\  \\\\\\  \\ \\  \\\\ \\  \\  \n"
           "  \\ \\  \\ \\  \\\\\\  \\ \\  \\\\ \\  \n"
           "   \\ \\__\\ \\_______\\ \\__\\\\ \\__\\\n"
           "    \\|__|\\|_______|\\|__| \\|__|   \n"
           "\033[0m");

    // Show boot menu options
    printf("\n1) Normally boot kernel\n");
    printf("2) About\n");
    printf("\nChoose an option (1 or 2): ");
    
    int choice;
    scanf("%d", &choice);  // Read user input

    // Handle the user's choice
    switch (choice) {
        case 1:
            printf("\nBooting the kernel normally...\n");
            break;
        case 2:
            printf("\n--- About ---\n");
            printf("Ion Kernel\n");
            printf("By kernuxx\n");
            break;
        default:
            printf("\nInvalid choice. Please select either 1 or 2.\n");
            break;
    }
}

#endif // BOOT_MENU_H
