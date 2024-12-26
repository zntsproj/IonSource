#ifndef BLOCK_IO_H
#define BLOCK_IO_H

#include <stdint.h>

int read_block(uint32_t block, void *buffer);
int write_block(uint32_t block, const void *buffer);

#endif // BLOCK_IO_H
