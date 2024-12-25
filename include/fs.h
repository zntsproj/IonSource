#ifndef FS_H
#define FS_H

#include <stddef.h> // For size_t

// Define the inode structure (minimal)
typedef struct inode {
    char name[256];           // File or directory name
    int is_directory;         // 1 if directory, 0 if file
    struct inode *parent;     // Parent directory
    struct inode *children;   // Pointer to children (subdirectories/files)
    struct superblock *sb;    // Pointer to associated superblock (NEW)
} inode_t;

// File system operations structure
typedef struct fs_operations {
    int (*mount)(const char *device);       // Mount operation
    int (*unmount)(void);                   // Unmount operation
    int (*read_inode)(inode_t *inode);      // Read inode from disk (NEW)
    int (*write_inode)(inode_t *inode);     // Write inode to disk (NEW)
    int (*sync)(void);                      // Sync filesystem (NEW)
} fs_operations_t;

// Superblock structure
typedef struct superblock {
    char fs_name[16];          // Filesystem name (e.g., "EXT4", "FAT32")
    size_t block_size;         // Block size in bytes
    size_t max_inodes;         // Maximum number of inodes
    size_t max_blocks;         // Maximum number of blocks
    inode_t *root_inode;       // Pointer to the root inode
    fs_operations_t *ops;      // Filesystem operations (pointer to fs_operations_t)
} superblock_t;

// Enum for file system types
enum fs_type {
    FS_TYPE_UNKNOWN,
    FS_TYPE_EXT4,
    FS_TYPE_FAT32,
};

// VFS functions (prototypes)
void vfs_init(void);
int vfs_mount(const char *device, fs_operations_t *fs_ops, enum fs_type fs_type);
int vfs_unmount(void);

#endif // FS_H
