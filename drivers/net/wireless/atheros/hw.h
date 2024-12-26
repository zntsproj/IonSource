#ifndef HW_H
#define HW_H

#include <stdint.h>
#include <stddef.h>

// Base addresses for memory-mapped I/O regions
#define RTC_SOC_BASE_ADDR        0x00080000
#define SOC_CORE_BASE_ADDR       0x00082000
#define WLAN_MAC_BASE_ADDR       0x00030000
#define CE_WRAPPER_BASE_ADDR     0x0004D000

// Registers offsets
#define SOC_CHIP_ID_ADDR         0x000000EC
#define SCRATCH_3_ADDR           0x00040050
#define FW_INDICATOR_ADDR        0x0004F00C
#define PCIE_INTR_CLR_ADDR       0x00000010

// Copy Engine (CE) base addresses
#define CE0_BASE_ADDR            0x0004A000
#define CE1_BASE_ADDR            0x0004A400
#define CE2_BASE_ADDR            0x0004A800
#define CE3_BASE_ADDR            0x0004AC00
#define CE4_BASE_ADDR            0x0004B000
#define CE5_BASE_ADDR            0x0004B400
#define CE6_BASE_ADDR            0x0004B800
#define CE7_BASE_ADDR            0x0004BC00

// Reset control masks
#define SOC_RESET_CONTROL_SI0_RST_MASK    0x00000001
#define SOC_RESET_CONTROL_CE_RST_MASK     0x00000100

// Hardware-specific functions

// Function to read a 32-bit register
static inline uint32_t hw_read_reg(uintptr_t base, uint32_t offset) {
    return *((volatile uint32_t *)(base + offset));
}

// Function to write a 32-bit register
static inline void hw_write_reg(uintptr_t base, uint32_t offset, uint32_t value) {
    *((volatile uint32_t *)(base + offset)) = value;
}

// Function to reset SOC
static inline void hw_reset_soc(uintptr_t base) {
    hw_write_reg(base, SOC_CHIP_ID_ADDR, SOC_RESET_CONTROL_SI0_RST_MASK);
    while (!(hw_read_reg(base, SOC_CHIP_ID_ADDR) & SOC_RESET_CONTROL_SI0_RST_MASK));
}

#endif // HW_H
