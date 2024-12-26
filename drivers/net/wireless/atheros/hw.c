#include "kernel.h"  // Kernel header from includes
#include "hw.h"      // Hardware access helpers (hw_read_reg, hw_write_reg)
#include <stdint.h>

// Device-specific definitions
#define ATHEROS_DEVICE_ID 0x003C  // Example device ID
#define ATHEROS_VENDOR_ID 0x168C  // Example vendor ID

// Register offsets (as examples)
#define ATHEROS_REG_CTRL       0x00  // Control register
#define ATHEROS_REG_STATUS     0x04  // Status register
#define ATHEROS_REG_TX_CTRL    0x08  // Transmit control register
#define ATHEROS_REG_RX_CTRL    0x0C  // Receive control register

// Driver structure
struct atheros_device {
    uintptr_t base_addr; // Base address of the device
    uint32_t irq;        // Interrupt request line
};

static struct atheros_device ath_dev;

// Function to initialize the device
static int atheros_init(uintptr_t base_addr, uint32_t irq) {
    ath_dev.base_addr = base_addr;
    ath_dev.irq = irq;

    // Reset the device
    hw_write_reg(ath_dev.base_addr, ATHEROS_REG_CTRL, 0x1);
    while (hw_read_reg(ath_dev.base_addr, ATHEROS_REG_STATUS) & 0x1) {
        // Wait for reset to complete
    }

    printk("[Atheros] Device initialized at base address 0x%lx with IRQ %d\n", base_addr, irq);
    return 0;
}

// Function to start the device
static void atheros_start(void) {
    hw_write_reg(ath_dev.base_addr, ATHEROS_REG_RX_CTRL, 0x1); // Enable RX
    hw_write_reg(ath_dev.base_addr, ATHEROS_REG_TX_CTRL, 0x1); // Enable TX
    printk("[Atheros] Device started. RX and TX enabled.\n");
}

// Function to stop the device
static void atheros_stop(void) {
    hw_write_reg(ath_dev.base_addr, ATHEROS_REG_RX_CTRL, 0x0); // Disable RX
    hw_write_reg(ath_dev.base_addr, ATHEROS_REG_TX_CTRL, 0x0); // Disable TX
    printk("[Atheros] Device stopped. RX and TX disabled.\n");
}

// Interrupt handler
static void atheros_isr(void) {
    uint32_t status = hw_read_reg(ath_dev.base_addr, ATHEROS_REG_STATUS);

    if (status & 0x01) {
        printk("[Atheros] RX interrupt\n");
        // in development
    }

    if (status & 0x02) {
        printk("[Atheros] TX interrupt\n");
        // in development
    }

    // Clear interrupt flags
    hw_write_reg(ath_dev.base_addr, ATHEROS_REG_STATUS, status);
}

// Module entry point
int atheros_driver_entry(uintptr_t base_addr, uint32_t irq) {
    int ret = atheros_init(base_addr, irq);
    if (ret != 0) {
        printk("[Atheros] Initialization failed\n");
        return ret;
    }

    atheros_start();
    printk("[Atheros] Driver loaded successfully\n");
    return 0;
}

// Module exit point
void atheros_driver_exit(void) {
    atheros_stop();
    printk("[Atheros] Driver unloaded successfully\n");
}


#ifdef TEST_DRIVER
int athmain(void) {
    uintptr_t base_addr = 0x00080000; // Example base address
    uint32_t irq = 5;                 // Example IRQ number

    atheros_driver_entry(base_addr, irq);
    atheros_isr();

    atheros_driver_exit();
    return 0;
}
#endif
