#ifndef IEEE802156_H
#define IEEE802156_H

#include "device.h"

// Define the bandwidth and frequency for IEEE 802.15.6
#define IEEE802156_BANDWIDTH 250000
#define IEEE802156_FREQUENCY 2400000000

// Structure to store the configuration settings for IEEE 802.15.6
typedef struct {
    uint32_t frequency;    // The frequency for the IEEE 802.15.6 communication
    uint32_t bandwidth;    // The bandwidth for the communication
    uint8_t modulation;    // The modulation type (0 for default)
    uint8_t power_level;   // The power level for the device
} ieee802156_config_t;

// Function to configure the IEEE 802.15.6 device
// It sets up frequency, bandwidth, modulation, and power level in the device registers
int configure_ieee802156(ieee802156_config_t *config) {
    if (!config) {
        return DEVICE_ERROR; // Return error if config is NULL
    }

    // Write the frequency, bandwidth, modulation, and power level to the respective registers
    REG_WRITE(0x40000000, config->frequency);  // Set the frequency in the device
    REG_WRITE(0x40000004, config->bandwidth);  // Set the bandwidth in the device
    REG_WRITE(0x40000008, config->modulation); // Set the modulation type
    REG_WRITE(0x4000000C, config->power_level); // Set the power level

    return DEVICE_OK;  // Return OK if the configuration is successful
}

#endif // IEEE802156_H
