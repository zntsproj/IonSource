#include "block_io.h"
#include <stdio.h>
#include <string.h>

#define DISK_SIZE 1024
#define BLOCK_SIZE 4096

static uint8_t disk[DISK_SIZE][BLOCK_SIZE];

int read_block(uint32_t block, void *buffer) {
    if (block >= DISK_SIZE) {
        return -1;
    }
    memcpy(buffer, disk[block], BLOCK_SIZE);
    return 0;
}

int write_block(uint32_t block, const void *buffer) {
    if (block >= DISK_SIZE) {
        return -1;
    }
    memcpy(disk[block], buffer, BLOCK_SIZE);
    return 0;
}
