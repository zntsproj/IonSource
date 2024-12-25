// Created by: ION Devteam
// 2022-2024 LICGX Team
// how to compile: gcc i2c.c -o i2c.o -I/your/path/to/ion_includes/

#include "i2c.h"
#include "io.h"
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

// I2C controller register base address and register offsets
#define I2C_BASE_ADDR   0x40003000
#define I2C_STATUS_REG  (*(volatile uint32_t *)(I2C_BASE_ADDR + 0x00))
#define I2C_CONTROL_REG (*(volatile uint32_t *)(I2C_BASE_ADDR + 0x04))
#define I2C_DATA_REG    (*(volatile uint32_t *)(I2C_BASE_ADDR + 0x08))
#define I2C_CLOCK_REG   (*(volatile uint32_t *)(I2C_BASE_ADDR + 0x0C))

// Status flags
#define I2C_STATUS_BUSY (1 << 0)
#define I2C_STATUS_DONE (1 << 1)

// Control flags
#define I2C_CONTROL_START (1 << 0)
#define I2C_CONTROL_STOP  (1 << 1)
#define I2C_CONTROL_ACK   (1 << 2)

// Error codes
#define I2C_SUCCESS 0
#define I2C_ERROR_TIMEOUT -1
#define I2C_ERROR_FAILED -2

// Timeout and retries
#define I2C_TIMEOUT 1000000 // Maximum number of iterations to wait
#define I2C_MAX_RETRIES 3


// Function to wait until I2C operation is done, with timeout
int wait_for_i2c_done(void) {
    int timeout = I2C_TIMEOUT;
    while (!(I2C_STATUS_REG & I2C_STATUS_DONE)) {
        if (--timeout <= 0) {
            printf("I2C operation timed out!\n");
            return I2C_ERROR_TIMEOUT;
        }
    }
    return 0;
}

// Initialize I2C controller
int i2c_init(void) {
    printf("Initializing I2C...\n");

    // Set I2C clock speed to 100kHz
    I2C_CLOCK_REG = 100000;

    // Enable automatic acknowledgment for data transfers
    I2C_CONTROL_REG |= I2C_CONTROL_ACK;

    return I2C_SUCCESS;
}

// Set the I2C clock speed
void i2c_set_speed(uint32_t speed) {
    printf("Setting I2C speed to %u Hz...\n", speed);
    I2C_CLOCK_REG = speed;
}

// Send data to I2C device
int i2c_send(i2c_device_t *device) {
    printf("Sending data to I2C device at address 0x%02X...\n", device->address);

    // Wait for I2C bus to be free
    while (I2C_STATUS_REG & I2C_STATUS_BUSY) {}

    // Generate a START condition
    I2C_CONTROL_REG |= I2C_CONTROL_START;

    // Send device address with the write flag (write operation)
    I2C_DATA_REG = (device->address << 1);  // Shift address and add write bit
    if (wait_for_i2c_done() != 0) {
        return I2C_ERROR_TIMEOUT;
    }

    // Send data byte-by-byte
    for (size_t i = 0; i < device->tx_size; i++) {
        I2C_DATA_REG = device->tx_buffer[i];
        if (wait_for_i2c_done() != 0) {
            return I2C_ERROR_TIMEOUT;
        }
    }

    // Generate a STOP condition to end communication
    I2C_CONTROL_REG |= I2C_CONTROL_STOP;

    return I2C_SUCCESS;
}

// Send multiple bytes to I2C device
int i2c_send_multiple(i2c_device_t *device) {
    printf("Sending multiple bytes to I2C device at address 0x%02X...\n", device->address);

    // Wait for I2C bus to be free
    while (I2C_STATUS_REG & I2C_STATUS_BUSY) {}

    // Generate a START condition
    I2C_CONTROL_REG |= I2C_CONTROL_START;

    // Send device address with the write flag (write operation)
    I2C_DATA_REG = (device->address << 1);
    if (wait_for_i2c_done() != 0) {
        return I2C_ERROR_TIMEOUT;
    }

    // Send data byte-by-byte
    for (size_t i = 0; i < device->tx_size; i++) {
        I2C_DATA_REG = device->tx_buffer[i];
        if (wait_for_i2c_done() != 0) {
            return I2C_ERROR_TIMEOUT;
        }
    }

    // Generate a STOP condition to end communication
    I2C_CONTROL_REG |= I2C_CONTROL_STOP;

    return I2C_SUCCESS;
}

// Receive data from I2C device
int i2c_receive(i2c_device_t *device) {
    printf("Receiving data from I2C device at address 0x%02X...\n", device->address);

    // Wait for I2C bus to be free
    while (I2C_STATUS_REG & I2C_STATUS_BUSY) {}

    // Generate a START condition
    I2C_CONTROL_REG |= I2C_CONTROL_START;

    // Send device address with the read flag (read operation)
    I2C_DATA_REG = (device->address << 1) | 0x01;  // Shift address and add read bit
    if (wait_for_i2c_done() != 0) {
        return I2C_ERROR_TIMEOUT;
    }

    // Receive data byte-by-byte
    for (size_t i = 0; i < device->rx_size; i++) {
        if (wait_for_i2c_done() != 0) {
            return I2C_ERROR_TIMEOUT;
        }
        device->rx_buffer[i] = I2C_DATA_REG;
    }

    // Generate a STOP condition to end communication
    I2C_CONTROL_REG |= I2C_CONTROL_STOP;

    return I2C_SUCCESS;
}

// Receive multiple bytes from I2C device
int i2c_receive_multiple(i2c_device_t *device) {
    printf("Receiving multiple bytes from I2C device at address 0x%02X...\n", device->address);

    // Wait for I2C bus to be free
    while (I2C_STATUS_REG & I2C_STATUS_BUSY) {}

    // Generate a START condition
    I2C_CONTROL_REG |= I2C_CONTROL_START;

    // Send device address with the read flag (read operation)
    I2C_DATA_REG = (device->address << 1) | 0x01;
    if (wait_for_i2c_done() != 0) {
        return I2C_ERROR_TIMEOUT;
    }

    // Receive data byte-by-byte
    for (size_t i = 0; i < device->rx_size; i++) {
        if (wait_for_i2c_done() != 0) {
            return I2C_ERROR_TIMEOUT;
        }
        device->rx_buffer[i] = I2C_DATA_REG;
    }

    // Generate a STOP condition to end communication
    I2C_CONTROL_REG |= I2C_CONTROL_STOP;

    return I2C_SUCCESS;
}

// Send data with retries
int i2c_send_with_retries(i2c_device_t *device) {
    int retries = 0;
    while (retries < I2C_MAX_RETRIES) {
        if (i2c_send(device) == I2C_SUCCESS) {
            return I2C_SUCCESS;
        }
        printf("I2C send failed, retrying... (%d/%d)\n", retries + 1, I2C_MAX_RETRIES);
        retries++;
    }
    return I2C_ERROR_FAILED;
}

// Main function, you can remove it

#define I2C_DEVICE_ADDR 0x50

int i2c_main() {
    if (i2c_init() != I2C_SUCCESS) {
        printf("I2C initialization failed!\n");
        return -1;
    }
    printf("I2C initialization succeeded.\n");

    uint8_t data_to_send[] = {0x01, 0x02, 0x03, 0x04};
    uint8_t data_received[4];

    i2c_device_t device;
    device.address = I2C_DEVICE_ADDR;
    device.tx_buffer = data_to_send;
    device.tx_size = sizeof(data_to_send);
    device.rx_buffer = data_received;
    device.rx_size = sizeof(data_received);

    if (i2c_send_with_retries(&device) == I2C_SUCCESS) {
        printf("Data sent successfully.\n");
    } else {
        printf("Data sending failed.\n");
        return -1;
    }

    if (i2c_receive(&device) == I2C_SUCCESS) {
        printf("Data received successfully: ");
        for (size_t i = 0; i < device.rx_size; i++) {
            printf("0x%02X ", device.rx_buffer[i]);
        }
        printf("\n");
    } else {
        printf("Data receiving failed.\n");
        return -1;
    }

    return 0;
}
