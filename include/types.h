#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

// Fixed-size data types
typedef uint8_t   u8;   // Unsigned 8-bit type
typedef int8_t    s8;   // Signed 8-bit type
typedef uint16_t  u16;  // Unsigned 16-bit type
typedef int16_t   s16;  // Signed 16-bit type
typedef uint32_t  u32;  // Unsigned 32-bit type
typedef int32_t   s32;  // Signed 32-bit type
typedef uint64_t  u64;  // Unsigned 64-bit type
typedef int64_t   s64;  // Signed 64-bit type

// Pointer types for various data
typedef void* ptr;    // Pointer to any type

// Structure types for working with addresses and ports
typedef uintptr_t address_t;  // Type for holding addresses (e.g., for I/O ports)

// Type for flags (e.g., for bitfields)
typedef u32 flag_t;

// Memory alignment attributes for structures
#define ALIGN4 __attribute__((aligned(4)))  // Align structure to 4-byte boundary
#define ALIGN8 __attribute__((aligned(8)))  // Align structure to 8-byte boundary

// Macros for bitwise operations
#define BIT(x) (1 << (x))                // Set a bit at position 'x'
#define GET_BIT(value, bit) (((value) >> (bit)) & 1)  // Get the bit at position 'bit'
#define SET_BIT(value, bit) ((value) |= BIT(bit))      // Set the bit at position 'bit'
#define CLEAR_BIT(value, bit) ((value) &= ~BIT(bit))   // Clear the bit at position 'bit'

// Type for interrupt handlers (function pointers)
typedef void (*handler_t)(void);

// Type for time intervals (in milliseconds or ticks)
typedef uint32_t time_t;  // Time type for intervals

#endif // TYPES_H
