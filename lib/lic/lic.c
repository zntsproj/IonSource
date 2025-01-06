#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LAST_FILE_PATH ".lic_last" // File to store the name of the last blank file
#define OUTPUT_FILE "output.bco"  // Default name for the compiled binary file

// Error messages for easier localization
#define ERROR_FILE_OPEN "Error opening file"
#define ERROR_SYNTAX "Syntax error in file"
#define ERROR_FILE_CREATE "Error creating file"
#define ERROR_FILE_READ "Error reading file"
#define ERROR_LAST_FILE_READ "Error reading last file name"

// Error handling with detailed messages
void handle_error(const char *message, const char *filename) {
    if (filename) {
        fprintf(stderr, "%s: %s\n", message, filename);
    } else {
        fprintf(stderr, "%s\n", message);
    }
    exit(1);
}

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
        handle_error(ERROR_FILE_OPEN, filename);
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
                handle_error("Closing bracket without matching opening bracket", filename);
                return 0;
            }

            char open = stack[top--];
            if ((ch == ')' && open != '(') ||
                (ch == '}' && open != '{') ||
                (ch == ']' && open != '[')) {
                fclose(file);
                handle_error("Mismatched brackets", filename);
                return 0;
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
        handle_error(ERROR_FILE_CREATE, filename);
    }
    fclose(file);

    // Save the filename to the last file tracker
    FILE *last_file = fopen(LAST_FILE_PATH, "w");
    if (!last_file) {
        handle_error(ERROR_FILE_CREATE, LAST_FILE_PATH);
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
        handle_error("No blank file found! Use `lic_create_blank` first.", NULL);
    }
    if (!fgets(filename, sizeof(filename), last_file)) {
        fclose(last_file);
        handle_error(ERROR_LAST_FILE_READ, NULL);
    }
    fclose(last_file);

    // Remove trailing newline characters from the filename
    filename[strcspn(filename, "\n")] = '\0';

    // Check if the last blank file exists
    FILE *src = fopen(filename, "r");
    if (!src) {
        handle_error("Error: file does not exist", filename);
    }

    // Check syntax before proceeding with compilation
    if (!lic_check_syntax(filename)) {
        fclose(src);
        handle_error("Syntax error in file", filename);
    }

    // Open the output file for writing
    FILE *out = fopen(OUTPUT_FILE, "w");
    if (!out) {
        fclose(src);
        handle_error("Error creating output file", OUTPUT_FILE);
    }

    // Read each character from the source file
    // Convert it to binary and write it to the output file
    int ch;
    while ((ch = fgetc(src)) != EOF) {
        if (ch < 0 || ch > 255) {
            fclose(src);
            fclose(out);
            handle_error("Invalid character encountered in file", filename);
        }
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
