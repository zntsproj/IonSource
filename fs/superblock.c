#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "block_io.h"  // Подключаем header файл с операциями с блоками

#define SUPERBLOCK_MAGIC 0xA1B2C3D4

struct superblock {
    uint32_t magic;
    uint32_t block_size;
    uint32_t total_blocks;
    uint32_t free_blocks;
    uint32_t root_inode;
    uint32_t block_bitmap;
    uint32_t inode_bitmap;
    uint32_t inode_table;
};

void init_superblock(struct superblock *sb, uint32_t total_blocks, uint32_t block_size) {
    sb->magic = SUPERBLOCK_MAGIC;
    sb->block_size = block_size;
    sb->total_blocks = total_blocks;
    sb->free_blocks = total_blocks;
    sb->root_inode = 1;
    sb->block_bitmap = 2;
    sb->inode_bitmap = 3;
    sb->inode_table = 4;
}

int read_superblock(struct superblock *sb, uint32_t block) {
    int status = read_block(block, sb);
    if (status != 0) {
        return -1;
    }

    if (sb->magic != SUPERBLOCK_MAGIC) {
        return -1;
    }

    return 0;
}

int write_superblock(struct superblock *sb, uint32_t block) {
    return write_block(block, sb);
}

void print_superblock(const struct superblock *sb) {
    printf("Superblock:\n");
    printf("  Magic number: 0x%X\n", sb->magic);
    printf("  Block size: %u bytes\n", sb->block_size);
    printf("  Total blocks: %u\n", sb->total_blocks);
    printf("  Free blocks: %u\n", sb->free_blocks);
    printf("  Root inode: %u\n", sb->root_inode);
    printf("  Block bitmap: Block %u\n", sb->block_bitmap);
    printf("  Inode bitmap: Block %u\n", sb->inode_bitmap);
    printf("  Inode table: Block %u\n", sb->inode_table);
}

int spmain() {
    struct superblock sb;
    init_superblock(&sb, 10000, 4096);  // 10000 blocks, 4096 bytes per block

    print_superblock(&sb);

    write_superblock(&sb, 0);

    if (read_superblock(&sb, 0) == 0) {
        print_superblock(&sb);
    }

    return 0;
}
