#ifndef TYPES_H
#define TYPES_H

// Fixed-size data types
typedef unsigned char      uint8_t;   // Unsigned 8-bit type
typedef signed char        int8_t;    // Signed 8-bit type
typedef unsigned short     uint16_t;  // Unsigned 16-bit type
typedef signed short       int16_t;   // Signed 16-bit type
typedef unsigned int       uint32_t;  // Unsigned 32-bit type
typedef signed int         int32_t;   // Signed 32-bit type
typedef unsigned long long uint64_t;  // Unsigned 64-bit type
typedef signed long long   int64_t;   // Signed 64-bit type

// Pointer types for various data
typedef void* ptr;    // Pointer to any type

// Structure types for working with addresses and ports
typedef unsigned long address_t;  // Type for holding addresses (e.g., for I/O ports)

// Type for flags (e.g., for bitfields)
typedef uint32_t flag_t;

// Memory alignment attributes for structures
#define ALIGN4 __attribute__((aligned(4)))  // Align structure to 4-byte boundary
#define ALIGN8 __attribute__((aligned(8)))  // Align structure to 8-byte boundary

// Macros for bitwise operations
#define BIT(x) (1U << (x))                // Set a bit at position 'x'
#define GET_BIT(value, bit) (((value) >> (bit)) & 1U)  // Get the bit at position 'bit'
#define SET_BIT(value, bit) ((value) |= BIT(bit))      // Set the bit at position 'bit'
#define CLEAR_BIT(value, bit) ((value) &= ~BIT(bit))   // Clear the bit at position 'bit'

// Type for interrupt handlers (function pointers)
typedef void (*handler_t)(void);

// Type for time intervals (in milliseconds or ticks)
typedef uint32_t time_t;  // Time type for intervals

#endif // TYPES_H
