#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LAST_FILE_PATH ".lic_last" // File to store the name of the last blank file
#define OUTPUT_FILE "output.bco"  // Default name for the compiled binary file

/**
 * Checks the syntax of the source file by verifying matching brackets.
 * Supports (), {}, and [].
 *
 * @param filename Name of the source file to check.
 * @return 1 if syntax is valid, 0 otherwise.
 */
int lic_check_syntax(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file for syntax check");
        return 0;
    }

    // Simple stack for bracket matching
    char stack[1024];
    int top = -1;

    int ch;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '(' || ch == '{' || ch == '[') {
            stack[++top] = ch;
        } else if (ch == ')' || ch == '}' || ch == ']') {
            if (top == -1) {
                fclose(file);
                return 0; // Closing bracket without matching opening bracket
            }

            char open = stack[top--];
            if ((ch == ')' && open != '(') ||
                (ch == '}' && open != '{') ||
                (ch == ']' && open != '[')) {
                fclose(file);
                return 0; // Mismatched brackets
            }
        }
    }

    fclose(file);
    return top == -1; // Valid if stack is empty
}

/**
 * Creates a blank code file and stores its name for future reference.
 * 
 * @param filename Name of the blank file to create.
 */
void lic_create_blank(const char *filename) {
    // Create an empty file
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error creating file");
        exit(1);
    }
    fclose(file);

    // Save the filename to the last file tracker
    FILE *last_file = fopen(LAST_FILE_PATH, "w");
    if (!last_file) {
        perror("Error saving last file name");
        exit(1);
    }
    fprintf(last_file, "%s\n", filename);
    fclose(last_file);

    // Notify the user that the blank file was created
    printf("Blank file created: %s\n", filename);
}

/**
 * Compiles the last created blank file into a binary output file.
 * 
 * Each character in the source file is converted to its binary 
 * representation (8 bits) and written to the output file.
 */
void lic_compile_last_blank() {
    char filename[256];

    // Read the name of the last blank file
    FILE *last_file = fopen(LAST_FILE_PATH, "r");
    if (!last_file) {
        fprintf(stderr, "No blank file found! Use `lic_create_blank` first.\n");
        exit(1);
    }
    if (!fgets(filename, sizeof(filename), last_file)) {
        perror("Error reading last file name");
        fclose(last_file);
        exit(1);
    }
    fclose(last_file);

    // Remove trailing newline characters from the filename
    filename[strcspn(filename, "\n")] = '\0';

    // Check if the last blank file exists
    FILE *src = fopen(filename, "r");
    if (!src) {
        fprintf(stderr, "Error: file %s does not exist.\n", filename);
        exit(1);
    }

    // Check syntax before proceeding with compilation
    if (!lic_check_syntax(filename)) {
        fprintf(stderr, "Syntax error in file: %s\n", filename);
        fclose(src);
        exit(1);
    }

    // Open the output file for writing
    FILE *out = fopen(OUTPUT_FILE, "w");
    if (!out) {
        perror("Error creating output file");
        fclose(src);
        exit(1);
    }

    // Read each character from the source file
    // Convert it to binary and write it to the output file
    int ch;
    while ((ch = fgetc(src)) != EOF) {
        for (int i = 7; i >= 0; i--) {
            fputc((ch & (1 << i)) ? '1' : '0', out);
        }
        fputc('\n', out); // Add a newline after each binary representation
    }

    fclose(src);
    fclose(out);

    // Notify the user about the successful compilation
    printf("Compiled %s -> %s\n", filename, OUTPUT_FILE);
}
