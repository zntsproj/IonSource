// ion/vfs.c

#include "fs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Global root inode for VFS
static inode_t *vfs_root = NULL;
static inode_t *mounted_fs = NULL;

// Initialize the Virtual File System (VFS)
void vfs_init(void) {
    // Initialize the root directory of the VFS
    vfs_root = (inode_t *)malloc(sizeof(inode_t));
    if (vfs_root == NULL) {
        printf("Error allocating memory for VFS root directory\n");
        return;
    }

    strcpy(vfs_root->name, "/");
    vfs_root->is_directory = 1;
    vfs_root->parent = NULL;
    vfs_root->children = NULL;

    printf("VFS initialized\n");
}

// Mount a file system to the VFS
int vfs_mount(const char *device, fs_operations_t *fs_ops, enum fs_type fs_type) {
    if (mounted_fs != NULL) {
        printf("A file system is already mounted\n");
        return -1; // Error: VFS is already mounted
    }

    // Allocate memory for the inode representing the mounted file system
    mounted_fs = (inode_t *)malloc(sizeof(inode_t));
    if (mounted_fs == NULL) {
        printf("Error allocating memory for the file system inode\n");
        return -1; // Memory allocation failure
    }

    // Initialize the inode with file system type and operations
    mounted_fs->is_directory = 1; // Let's assume the mount point is a directory
    mounted_fs->parent = NULL;
    mounted_fs->children = NULL;

    // Call the file system's mount operation
    int result = fs_ops->mount(device);
    if (result != 0) {
        printf("Failed to mount the file system on device %s\n", device);
        free(mounted_fs);
        mounted_fs = NULL;
        return -1; // Mount failure
    }

    printf("File system mounted successfully on device %s\n", device);
    return 0; // Success
}

// Unmount the file system from the VFS
int vfs_unmount(void) {
    if (mounted_fs == NULL) {
        printf("No file system is currently mounted\n");
        return -1; // Error: No file system mounted
    }

    // Call the file system's unmount operation
    int result = mounted_fs->is_directory; // Just an example, no real unmount function here
    if (result != 0) {
        printf("Failed to unmount the file system\n");
        return -1; // Unmount failure
    }

    // Free the resources associated with the mounted file system
    free(mounted_fs);
    mounted_fs = NULL;

    printf("File system unmounted successfully\n");
    return 0; // Success
}
