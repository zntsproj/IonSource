// netdev.h - Header file for network devices
#ifndef NETDEV_H
#define NETDEV_H

#include <stdint.h>
#include <stddef.h>

// Network device structure
struct net_device {
    char name[16];              // Device name (e.g., "eth0", "wlan0")
    uint8_t mac_address[6];     // MAC address
    uint32_t ip_address;        // IP address
    void (*transmit)(struct net_device *dev, const uint8_t *data, size_t len); // Transmit function
    void (*receive)(struct net_device *dev, uint8_t *buffer, size_t len);      // Receive function
    void *driver_data;          // Driver-specific data
};

// Initialize the network device
void net_device_init(struct net_device *dev, const char *name, const uint8_t *mac_address, uint32_t ip_address);

// Set transmit callback
void net_device_set_transmit_callback(struct net_device *dev, void (*transmit)(struct net_device *, const uint8_t *, size_t));

// Set receive callback
void net_device_set_receive_callback(struct net_device *dev, void (*receive)(struct net_device *, uint8_t *, size_t));

#endif // NETDEV_H