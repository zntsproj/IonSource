#ifndef RTL8188EU_H
#define RTL8188EU_H

#include "io.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>

// Define RTL8188EU I/O port addresses
#define RTL8188EU_PORT_CMD       0x03F8  // Command port
#define RTL8188EU_PORT_STATUS    0x03F9  // Status port
#define RTL8188EU_PORT_SSID      0x03FA  // SSID port
#define RTL8188EU_PORT_PASSWORD  0x03FB  // Password port

// Define commands for Wi-Fi module
#define CMD_CONNECT              0x01    // Command to initiate connection
#define CMD_DISCONNECT           0x02    // Command to disconnect

// Define status codes
#define STATUS_CONNECTED         0x01    // Status: connected
#define STATUS_DISCONNECTED      0x00    // Status: disconnected

// Default timeout values (in arbitrary units)
#define DEFAULT_CONNECT_TIMEOUT  1000
#define DEFAULT_DISCONNECT_TIMEOUT 1000

// Function to connect to a Wi-Fi network
static inline int rtl8188eu_connect(const char *ssid, const char *password, int timeout_ms) {
    // Validate input parameters
    if (!ssid || strlen(ssid) == 0 || strlen(ssid) >= 256) {
        return -1;  // Invalid SSID
    }
    if (password && strlen(password) >= 256) {
        return -2;  // Invalid password
    }

    // Send SSID to the device
    size_t ssid_length = strlen(ssid);
    for (size_t i = 0; i < ssid_length; i++) {
        outb(RTL8188EU_PORT_SSID, ssid[i]);
    }
    outb(RTL8188EU_PORT_SSID, '\0');  // Null-terminate the SSID in the hardware

    // Send password to the device (if provided)
    if (password) {
        size_t password_length = strlen(password);
        for (size_t i = 0; i < password_length; i++) {
            outb(RTL8188EU_PORT_PASSWORD, password[i]);
        }
        outb(RTL8188EU_PORT_PASSWORD, '\0');  // Null-terminate the password
    }

    // Issue the connect command
    outb(RTL8188EU_PORT_CMD, CMD_CONNECT);

    // Wait for the connection to be established
    for (int timeout = timeout_ms; timeout > 0; timeout--) {
        uint8_t status = inb(RTL8188EU_PORT_STATUS);
        if (status == STATUS_CONNECTED) {
            return 0;  // Connection successful
        }
    }

    // If we reach here, the connection timed out
    return -3;  // Connection failed due to timeout
}

// Function to disconnect from the Wi-Fi network
static inline int rtl8188eu_disconnect(int timeout_ms) {
    // Issue the disconnect command
    outb(RTL8188EU_PORT_CMD, CMD_DISCONNECT);

    // Wait for the device to confirm disconnection
    for (int timeout = timeout_ms; timeout > 0; timeout--) {
        uint8_t status = inb(RTL8188EU_PORT_STATUS);
        if (status == STATUS_DISCONNECTED) {
            return 0;  // Disconnection successful
        }
    }

    // If we reach here, the disconnection timed out
    return -1;  // Disconnection failed due to timeout
}

// Wrapper functions with default timeouts
static inline int rtl8188eu_connect_default(const char *ssid, const char *password) {
    return rtl8188eu_connect(ssid, password, DEFAULT_CONNECT_TIMEOUT);
}

static inline int rtl8188eu_disconnect_default(void) {
    return rtl8188eu_disconnect(DEFAULT_DISCONNECT_TIMEOUT);
}

#endif // RTL8188EU_H
