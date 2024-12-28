#ifndef IEEE80222_H
#define IEEE80222_H

#include <stdint.h>
#include <stddef.h>

// Define some basic constants for IEEE 802.22

// Frequency range for IEEE 802.22 (TV White Spaces)
#define IEEE80222_FREQUENCY_MIN 54e6    // 54 MHz
#define IEEE80222_FREQUENCY_MAX 862e6   // 862 MHz

// Maximum number of stations (devices) that can be supported by a base station
#define IEEE80222_MAX_STATIONS 256

// Maximum length of a data packet (in bytes)
#define IEEE80222_MAX_PACKET_SIZE 2048

// Structure for the IEEE 802.22 header
typedef struct {
    uint8_t  frame_type;      // Frame type: Data, ACK, etc.
    uint16_t frame_length;    // Frame length (excluding header)
    uint32_t source_address;  // Source address (device ID)
    uint32_t destination_address; // Destination address (device ID)
    uint32_t timestamp;       // Timestamp for synchronization
} ieee80222_header_t;

// Structure for a basic station in IEEE 802.22
typedef struct {
    uint32_t station_id;      // Unique station ID
    uint32_t address;         // MAC address of the station
    uint8_t  signal_strength; // Signal strength (dBm)
    uint8_t  channel;         // Channel currently used by the station
    uint32_t last_seen;       // Timestamp of the last communication
} ieee80222_station_t;

// Structure for the base station in IEEE 802.22
typedef struct {
    uint32_t base_station_id; // Unique base station ID
    uint32_t frequency;       // Frequency currently used by the base station
    ieee80222_station_t stations[IEEE80222_MAX_STATIONS]; // Connected stations
} ieee80222_base_station_t;

// Function prototypes for IEEE 802.22
void ieee80222_init(void);                                 // Initialize IEEE 802.22 system
void ieee80222_create_base_station(uint32_t base_station_id, uint32_t frequency); // Create base station
void ieee80222_add_station(uint32_t base_station_id, uint32_t station_id, uint32_t address); // Add station to base station
void ieee80222_send_data(uint32_t source, uint32_t destination, uint8_t *data, size_t data_len); // Send data from one station to another
void ieee80222_receive_data(uint8_t *buffer, size_t buffer_len); // Receive data at base station

// Utility functions
int ieee80222_get_signal_strength(uint32_t station_id); // Get signal strength of a station
void ieee80222_scan_networks(void); // Scan for available IEEE 802.22 networks

#endif // IEEE80222_H
