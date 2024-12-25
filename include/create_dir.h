#ifndef CREATE_DIR_H
#define CREATE_DIR_H

#include <stdio.h>  // file operations
#include <stdlib.h>
#include <sys/stat.h> // Yeah, i know that using linux headers in own kernel is so dumb... But it's just a little kernel!

// Function for catalog creation
// 0 for success, 1 for error
int create_dir(const char *dir_name) {
    int result = mkdir(dir_name, 0777);

    if (result != 0) {
        perror("Error creating directory"); // Error message
        return -1; // Return error if catalog not created
    }

    return 0; // Return success
}

#endif // CREATE_DIR_H
