#ifndef IEEE80222_H
#define IEEE80222_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#define IEEE80222_FREQUENCY_MIN 54e6
#define IEEE80222_FREQUENCY_MAX 862e6
#define IEEE80222_MAX_STATIONS 256
#define IEEE80222_MAX_PACKET_SIZE 2048

typedef struct {
    uint8_t  frame_type;
    uint16_t frame_length;
    uint32_t source_address;
    uint32_t destination_address;
    uint32_t timestamp;
} ieee80222_header_t;

typedef struct {
    uint32_t station_id;
    uint32_t address;
    uint8_t  signal_strength;
    uint8_t  channel;
    uint32_t last_seen;
} ieee80222_station_t;

typedef struct {
    uint32_t base_station_id;
    uint32_t frequency;
    ieee80222_station_t stations[IEEE80222_MAX_STATIONS];
} ieee80222_base_station_t;

void ieee80222_init(void);
void ieee80222_create_base_station(uint32_t base_station_id, uint32_t frequency);
void ieee80222_add_station(uint32_t base_station_id, uint32_t station_id, uint32_t address);
void ieee80222_send_data(uint32_t source, uint32_t destination, uint8_t *data, size_t data_len);
void ieee80222_receive_data(uint8_t *buffer, size_t buffer_len);
int ieee80222_get_signal_strength(uint32_t station_id);
void ieee80222_scan_networks(void);

static ieee80222_base_station_t base_stations[10];

void ieee80222_init(void) {
    for (int i = 0; i < 10; i++) {
        base_stations[i].base_station_id = 0;
        base_stations[i].frequency = 0;
        for (int j = 0; j < IEEE80222_MAX_STATIONS; j++) {
            base_stations[i].stations[j].station_id = 0;
            base_stations[i].stations[j].address = 0;
        }
    }
}

void ieee80222_create_base_station(uint32_t base_station_id, uint32_t frequency) {
    for (int i = 0; i < 10; i++) {
        if (base_stations[i].base_station_id == 0) {
            base_stations[i].base_station_id = base_station_id;
            base_stations[i].frequency = frequency;
            return;
        }
    }
}

void ieee80222_add_station(uint32_t base_station_id, uint32_t station_id, uint32_t address) {
    for (int i = 0; i < 10; i++) {
        if (base_stations[i].base_station_id == base_station_id) {
            for (int j = 0; j < IEEE80222_MAX_STATIONS; j++) {
                if (base_stations[i].stations[j].station_id == 0) {
                    base_stations[i].stations[j].station_id = station_id;
                    base_stations[i].stations[j].address = address;
                    return;
                }
            }
            return;
        }
    }
}

void ieee80222_send_data(uint32_t source, uint32_t destination, uint8_t *data, size_t data_len) {
}

void ieee80222_receive_data(uint8_t *buffer, size_t buffer_len) {
}

int ieee80222_get_signal_strength(uint32_t station_id) {
    return -65;
}

void ieee80222_scan_networks(void) {
}

#endif // IEEE80222_H
