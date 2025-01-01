#ifndef DEVICE_H
#define DEVICE_H

#include <stdint.h>
#include <stddef.h>

// General macros for working with device registers
#define REG_READ(addr)     (*(volatile uint32_t *)(addr))  // Read from a register
#define REG_WRITE(addr, val) (*(volatile uint32_t *)(addr) = (val))  // Write to a register

// Device status codes
typedef enum {
    DEVICE_OK = 0,          // Device is working fine
    DEVICE_ERROR = -1,      // Device error occurred
    DEVICE_BUSY = -2,       // Device is busy, cannot perform the operation
    DEVICE_NOT_FOUND = -3,  // Device not found
    DEVICE_TIMEOUT = -4     // Timeout during operation
} device_status_t;

// Device state flags
typedef enum {
    DEVICE_STATE_IDLE = 0,  // Device is idle, not processing any task
    DEVICE_STATE_BUSY = 1,  // Device is busy, performing an operation
    DEVICE_STATE_ERROR = 2  // Device is in error state
} device_state_t;

// Device types
typedef enum {
    DEVICE_TYPE_UNKNOWN,    // Unknown device type
    DEVICE_TYPE_I2C,        // I2C device
    DEVICE_TYPE_UART,       // UART device
    DEVICE_TYPE_SPI,        // SPI device
    DEVICE_TYPE_GPIO,       // GPIO device
    DEVICE_TYPE_TIMER,      // Timer device
    DEVICE_TYPE_INTERRUPT,  // Interrupt-enabled device
} device_type_t;

// Device structure that represents a hardware device
typedef struct {
    uint32_t base_addr;       // Base address of the device's registers
    device_state_t status;    // Status of the device
    uint32_t control;         // Control registers for the device
    uint32_t irq_vector;      // IRQ vector for interrupt-enabled devices
    device_type_t type;       // Type of the device (I2C, UART, etc.)
    void (*init)(void);       // Device initialization function
    void (*deinit)(void);     // Device de-initialization function
    device_status_t (*read)(uint8_t *buffer, size_t size);  // Read data from the device
    device_status_t (*write)(const uint8_t *buffer, size_t size);  // Write data to the device
    device_status_t (*ioctl)(uint32_t cmd, void *arg);     // Control the device (e.g., set parameters)
} device_t;

// Macros to initialize, deinitialize, read, write, and control devices
#define DEVICE_INIT(dev)      ((dev)->init ? (dev)->init() : DEVICE_OK)  // Initialize the device
#define DEVICE_DEINIT(dev)    ((dev)->deinit ? (dev)->deinit() : DEVICE_OK)  // Deinitialize the device
#define DEVICE_READ(dev, buf, size)   ((dev)->read ? (dev)->read(buf, size) : DEVICE_ERROR)  // Read from the device
#define DEVICE_WRITE(dev, buf, size)  ((dev)->write ? (dev)->write(buf, size) : DEVICE_ERROR)  // Write to the device
#define DEVICE_IOCTL(dev, cmd, arg)   ((dev)->ioctl ? (dev)->ioctl(cmd, arg) : DEVICE_ERROR)  // Control the device

// Device management functions
device_status_t device_init(device_t *dev);  // Initialize the device
device_status_t device_deinit(device_t *dev);  // De-initialize the device
device_status_t device_read(device_t *dev, uint8_t *buffer, size_t size);  // Read data from the device
device_status_t device_write(device_t *dev, const uint8_t *buffer, size_t size);  // Write data to the device
device_status_t device_ioctl(device_t *dev, uint32_t cmd, void *arg);  // Control the device (IOCTL)
device_status_t device_wait_for_ready(device_t *dev);  // Wait until the device is ready to perform an operation
device_status_t device_set_irq(device_t *dev, uint32_t irq_vector);  // Set IRQ vector for interrupt-enabled devices

// Common IOCTL commands for device control
#define IOCTL_CMD_RESET       0x01  // Reset the device
#define IOCTL_CMD_SET_SPEED   0x02  // Set the device speed (e.g., baud rate, clock speed)
#define IOCTL_CMD_ENABLE_IRQ  0x03  // Enable IRQ (interrupts) for the device
#define IOCTL_CMD_DISABLE_IRQ 0x04  // Disable IRQ for the device

// Functions for specific device types (I2C, UART, SPI, GPIO)
device_status_t i2c_device_init(device_t *dev);  // Initialize an I2C device
device_status_t uart_device_init(device_t *dev);  // Initialize a UART device
device_status_t spi_device_init(device_t *dev);  // Initialize an SPI device
device_status_t gpio_device_init(device_t *dev);  // Initialize a GPIO device

// Example of an extended function for waiting until a device is ready
device_status_t device_wait_for_ready(device_t *dev) {
    // This function waits until the device is ready to process a new operation
    if (dev->status == DEVICE_STATE_BUSY) {
        while (dev->status == DEVICE_STATE_BUSY) {
            // Wait until the device becomes ready
        }
    }
    return DEVICE_OK;
}

// Set IRQ for a device with interrupts
device_status_t device_set_irq(device_t *dev, uint32_t irq_vector) {
    // This function sets the IRQ vector for interrupt-enabled devices
    if (dev->type != DEVICE_TYPE_INTERRUPT) {
        return DEVICE_ERROR;  // Return error if device doesn't support interrupts
    }
    dev->irq_vector = irq_vector;  // Set the IRQ vector
    REG_WRITE(dev->base_addr + 0x10, irq_vector);
    return DEVICE_OK;
}

device_status_t i2c_device_init(device_t *dev) {
    // Initialize I2C device
    if (!dev) return DEVICE_ERROR;
    dev->status = DEVICE_STATE_IDLE;
    return DEVICE_OK;
}

device_status_t uart_device_init(device_t *dev) {
    // Initialize UART device
    if (!dev) return DEVICE_ERROR;
    dev->status = DEVICE_STATE_IDLE;
    return DEVICE_OK;
}

device_status_t spi_device_init(device_t *dev) {
    // Initialize SPI device
    if (!dev) return DEVICE_ERROR;
    dev->status = DEVICE_STATE_IDLE;
    return DEVICE_OK;
}

device_status_t gpio_device_init(device_t *dev) {
    // Initialize GPIO device
    if (!dev) return DEVICE_ERROR;
    dev->status = DEVICE_STATE_IDLE;
    return DEVICE_OK;
}

#endif // DEVICE_H
