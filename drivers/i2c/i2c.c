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

// I2C speeds
#define I2C_SPEED_STANDARD 100000  // Standard mode (100kHz)
#define I2C_SPEED_FAST 400000     // Fast mode (400kHz)
#define I2C_SPEED_HIGH 3400000    // High-speed mode (3.4Mbps)

// Function to scan the I2C bus for devices (addresses 0x00 to 0x7F)
int i2c_scan_bus(void) {
    printf("Scanning I2C bus...\n");

    for (uint8_t address = 0x00; address < 0x80; address++) {
        i2c_device_t device;
        device.address = address;
        device.tx_buffer = NULL;
        device.tx_size = 0;
        device.rx_buffer = NULL;
        device.rx_size = 0;

        // Attempt to send a dummy byte to check if the device responds
        if (i2c_send(&device) == I2C_SUCCESS) {
            printf("Found device at address 0x%02X\n", address);
        }
    }

    return 0;
}

// Function to write to a specific register on the I2C device
int i2c_write_register(i2c_device_t *device, uint8_t register_address, uint8_t value) {
    printf("Writing 0x%02X to register 0x%02X of device 0x%02X...\n", value, register_address, device->address);

    uint8_t buffer[] = {register_address, value};
    device->tx_buffer = buffer;
    device->tx_size = sizeof(buffer);

    // Send the data (address + value)
    if (i2c_send(device) == I2C_SUCCESS) {
        printf("Write successful!\n");
        return I2C_SUCCESS;
    } else {
        printf("Write failed!\n");
        return I2C_ERROR_FAILED;
    }
}

// Function to read a register from an I2C device
int i2c_read_register(i2c_device_t *device, uint8_t register_address, uint8_t *value) {
    printf("Reading from register 0x%02X of device 0x%02X...\n", register_address, device->address);

    uint8_t tx_buffer[] = {register_address};
    device->tx_buffer = tx_buffer;
    device->tx_size = sizeof(tx_buffer);
    device->rx_buffer = value;
    device->rx_size = 1;

    // Send the register address and then receive the data
    if (i2c_send(device) == I2C_SUCCESS && i2c_receive(device) == I2C_SUCCESS) {
        printf("Read successful: 0x%02X\n", *value);
        return I2C_SUCCESS;
    } else {
        printf("Read failed!\n");
        return I2C_ERROR_FAILED;
    }
}

// Function to check I2C bus status
int i2c_check_bus_status(void) {
    printf("Checking I2C bus status...\n");

    // Check if the bus is busy or if an error condition is present
    if (I2C_STATUS_REG & I2C_STATUS_BUSY) {
        printf("I2C bus is busy.\n");
        return I2C_ERROR_FAILED;
    }

    printf("I2C bus is available.\n");
    return I2C_SUCCESS;
}

// Function to wait until I2C operation is done, with timeout
int wait_for_i2c_done_with_timeout(int timeout) {
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
    I2C_CLOCK_REG = I2C_SPEED_STANDARD;

    // Enable automatic acknowledgment for data transfers
    I2C_CONTROL_REG |= I2C_CONTROL_ACK;

    return I2C_SUCCESS;
}

// Set the I2C clock speed
void i2c_set_speed(uint32_t speed) {
    printf("Setting I2C speed to %u Hz...\n", speed);
    I2C_CLOCK_REG = speed;
}

// Log I2C registers (useful for debugging)
void i2c_log_registers(void) {
    printf("I2C STATUS: 0x%08X\n", I2C_STATUS_REG);
    printf("I2C CONTROL: 0x%08X\n", I2C_CONTROL_REG);
    printf("I2C DATA: 0x%08X\n", I2C_DATA_REG);
    printf("I2C CLOCK: 0x%08X\n", I2C_CLOCK_REG);
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
    if (wait_for_i2c_done_with_timeout(I2C_TIMEOUT) != 0) {
        return I2C_ERROR_TIMEOUT;
    }

    // Send data byte-by-byte
    for (size_t i = 0; i < device->tx_size; i++) {
        I2C_DATA_REG = device->tx_buffer[i];
        if (wait_for_i2c_done_with_timeout(I2C_TIMEOUT) != 0) {
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
    if (wait_for_i2c_done_with_timeout(I2C_TIMEOUT) != 0) {
        return I2C_ERROR_TIMEOUT;
    }

    // Send data byte-by-byte
    for (size_t i = 0; i < device->tx_size; i++) {
        I2C_DATA_REG = device->tx_buffer[i];
        if (wait_for_i2c_done_with_timeout(I2C_TIMEOUT) != 0) {
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
    if (wait_for_i2c_done_with_timeout(I2C_TIMEOUT) != 0) {
        return I2C_ERROR_TIMEOUT;
    }

    // Receive data byte-by-byte
    for (size_t i = 0; i < device->rx_size; i++) {
        if (wait_for_i2c_done_with_timeout(I2C_TIMEOUT) != 0) {
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
    if (wait_for_i2c_done_with_timeout(I2C_TIMEOUT) != 0) {
        return I2C_ERROR_TIMEOUT;
    }

    // Receive data byte-by-byte
    for (size_t i = 0; i < device->rx_size; i++) {
        if (wait_for_i2c_done_with_timeout(I2C_TIMEOUT) != 0) {
            return I2C_ERROR_TIMEOUT;
        }
        device->rx_buffer[i] = I2C_DATA_REG;
    }

    // Generate a STOP condition to end communication
    I2C_CONTROL_REG |= I2C_CONTROL_STOP;

    return I2C_SUCCESS;
}

// Send data with retries
int i2c_send_with_retries(i2c_device_t *device, int max_retries) {
    int retries = 0;
    while (retries < max_retries) {
        if (i2c_send(device) == I2C_SUCCESS) {
            return I2C_SUCCESS;
        }
        printf("I2C send failed, retrying... (%d/%d)\n", retries + 1, max_retries);
        retries++;
    }
    return I2C_ERROR_FAILED;
}

// Function to delete I2C device (simply clears the buffers and address)
void delete_i2c_device(i2c_device_t *device) {
    printf("Deleting I2C device at address 0x%02X...\n", device->address);
    device->address = 0x00;  // Reset address
    device->tx_buffer = NULL; // Reset transmit buffer
    device->rx_buffer = NULL; // Reset receive buffer
    device->tx_size = 0;      // Reset transmit size
    device->rx_size = 0;      // Reset receive size
}

#define I2C_DEVICE_ADDR 0x50

// Main function...

int i2c_main() {
    if (i2c_init() != I2C_SUCCESS) {
        printf("I2C initialization failed!\n");
        return -1;
    }

    // Scan the I2C bus for devices
    i2c_scan_bus();

    uint8_t data_to_send[] = {0x01, 0x02, 0x03, 0x04};
    uint8_t data_received[4];

    i2c_device_t device;
    device.address = I2C_DEVICE_ADDR;
    device.tx_buffer = data_to_send;
    device.tx_size = sizeof(data_to_send);
    device.rx_buffer = data_received;
    device.rx_size = sizeof(data_received);

    if (i2c_send_with_retries(&device, I2C_MAX_RETRIES) == I2C_SUCCESS) {
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

    // Example of register write/read
    uint8_t reg_value;
    if (i2c_write_register(&device, 0x10, 0xFF) == I2C_SUCCESS) {
        printf("Write operation successful.\n");
    } else {
        printf("Write operation failed.\n");
    }

    if (i2c_read_register(&device, 0x10, &reg_value) == I2C_SUCCESS) {
        printf("Read operation successful. Register value: 0x%02X\n", reg_value);
    } else {
        printf("Read operation failed.\n");
    }

    // Delete the device after use
    delete_i2c_device(&device);

    return 0;
}
