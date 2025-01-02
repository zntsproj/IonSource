// fsapi.h

#ifndef FSAPI_H
#define FSAPI_H

#include <stddef.h> // For size_t type
#include <stdio.h>  // For basic file operations
#include <string.h> // For memset

// Function to create a file with the specified name.
// Parameters:
//   - filename: The name of the file to create (null-terminated string).
// Returns:
//   - 0 on success, non-zero error code on failure.
int create_file(const char *filename) {
    if (!filename) return -1; // Error: Null filename

    FILE *file = fopen(filename, "wb");
    if (!file) return -2; // Error: Could not create file

    fclose(file);
    return 0; // Success
}

// Function to create a file with the specified name and fill it with zeros up to a given size.
// Parameters:
//   - filename: The name of the file to create (null-terminated string).
//   - size: The size of the file in bytes, filled with zeros.
// Returns:
//   - 0 on success, non-zero error code on failure.
int create_filew(const char *filename, size_t size) {
    if (!filename) return -1; // Error: Null filename

    FILE *file = fopen(filename, "wb");
    if (!file) return -2; // Error: Could not create file

    char buffer[1024] = {0}; // Buffer to write zeros
    size_t remaining = size;

    while (remaining > 0) {
        size_t chunk_size = (remaining > sizeof(buffer)) ? sizeof(buffer) : remaining;
        if (fwrite(buffer, 1, chunk_size, file) != chunk_size) {
            fclose(file);
            return -3; // Error: Could not write to file
        }
        remaining -= chunk_size;
    }

    fclose(file);
    return 0; // Success
}

// Function to delete a file with the specified name.
// Parameters:
//   - filename: The name of the file to delete (null-terminated string).
// Returns:
//   - 0 on success, non-zero error code on failure.
int delete_file(const char *filename) {
    if (!filename) return -1; // Error: Null filename

    if (remove(filename) != 0) return -2; // Error: Could not delete file

    return 0; // Success
}

// Function to create a mknod-like file containing user-defined configurations.
// Parameters:
//   - filename: The name of the mknod file to create (null-terminated string).
//   - config: The configuration data to write into the file (null-terminated string).
// Returns:
//   - 0 on success, non-zero error code on failure.
int create_mknod(const char *filename, const char *config) {
    if (!filename || !config) return -1; // Error: Null parameters

    FILE *file = fopen(filename, "wb");
    if (!file) return -2; // Error: Could not create file

    if (fwrite(config, 1, strlen(config), file) != strlen(config)) {
        fclose(file);
        return -3; // Error: Could not write to file
    }

    fclose(file);
    return 0; // Success
}

#endif // FSAPI_H
