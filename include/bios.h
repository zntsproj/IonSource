#ifndef ION_BIOS_H
#define ION_BIOS_H

#include <stdint.h>

// BIOS interrupts
#define BIOS_VIDEO_INT 0x10
#define BIOS_DISK_INT  0x13
#define BIOS_MEM_INT   0x15

/**
 * Perform a BIOS interrupt call.
 * @param interrupt - The interrupt number (e.g.0x10 for video).
 * @param ax - Value for AX register.
 * @param bx - Value for BX register.
 * @param cx - Value for CX register.
 * @param dx - Value for DX register.
 */
static inline void bios_call(uint8_t interrupt, uint16_t ax, uint16_t bx, uint16_t cx, uint16_t dx) {
    asm volatile (
        "int %[int_num]"
        :                      
        : [int_num] "i"(interrupt), 
          "a"(ax),          
          "b"(bx),     
          "c"(cx),          
          "d"(dx)           
        : "memory", "cc"      
    );
}

/**
 * Get the amount of memory in kilobytes using BIOS int 0x15.
 * @return The size of available memory in kilobytes.
 */
static inline uint32_t get_memory_size() {
    uint32_t mem_kb;

    // AX = 0xE801 is the function to query extended memory
    __asm__ __volatile__ (
        "movw $0xE801, %%ax\n\t"  // Load function code into AX
        "int $0x15\n\t"           // Call BIOS interrupt 0x15
        "shll $16, %%ebx\n\t"     // Shift BX (high word) left by 16 bits
        "addl %%ebx, %%eax\n\t"   // Add AX (low word) to BX (high word)
        : "=a"(mem_kb)            // Output: total memory size in EAX
        :                         // No input operands
        : "ebx", "ecx", "edx", "memory", "cc"  // Clobbers
    );

    return mem_kb;  // Return the total memory size in kilobytes
}

#endif // ION_BIOS_H
