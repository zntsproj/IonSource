#ifndef CREATE_FLT_H
#define CREATE_FLT_H

#include <stdio.h>  // For file operations
#include <stdlib.h> // For error handling

// Function to create a file
// Returns 0 on success, -1 on failure
int create_flt(const char *filename) {
    // Open the file in write mode (will overwrite if file exists)
    FILE *file = fopen(filename, "w");

    // Check if the file was created successfully
    if (file == NULL) {
        perror("Error creating file"); // Print error message
        return -1; // Return failure code if file can't be created
    }

    // Optionally, you could write some initial data into the file here
    // fwrite(data, sizeof(char), length, file);

    // Close the file after operations are complete
    fclose(file);

    return 0; // Return success code
}

#endif // CREATE_FLT_H
