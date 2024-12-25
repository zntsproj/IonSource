#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include <stddef.h>

// Define some constants
#define I2C_MAX_BUFFER_SIZE 64
#define I2C_SUCCESS 0
#define I2C_ERR_TIMEOUT -1
#define I2C_ERR_BUSY -2

// Structure to hold information about an I2C device
typedef struct {
    uint8_t address;      // 7-bit I2C address
    uint8_t *tx_buffer;   // Transmit buffer
    uint8_t *rx_buffer;   // Receive buffer
    size_t tx_size;       // Size of data to send
    size_t rx_size;       // Size of data to receive
} i2c_device_t;

// Function declarations
int i2c_init(void);
int i2c_send(i2c_device_t *device);
int i2c_receive(i2c_device_t *device);
void i2c_set_speed(uint32_t speed); // Set clock speed (e.g., 100kHz, 400kHz, etc.)

#endif // I2C_H
