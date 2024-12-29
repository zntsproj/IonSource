#ifndef IEEE80211_H
#define IEEE80211_H

#include <stdint.h>

#define TX_FRAME_CONTROL_REG    0x4000
#define TX_PACKET_STATUS_REG    0x4004
#define RX_FRAME_CONTROL_REG    0x4008
#define RX_PACKET_STATUS_REG    0x400C
#define MAC_ADDR_REG            0x4010
#define CHANNEL_CTRL_REG        0x4014
#define CARRIER_SENSE_REG       0x4018
#define LINK_QUALITY_REG        0x401C
#define POWER_MGMT_REG          0x4020
#define DATA_RATE_REG           0x4024
#define SECURITY_CTRL_REG       0x4028
#define BEACON_INTERVAL_REG     0x402C
#define SHORT_PREAMBLE_CTRL_REG 0x4030
#define TX_POWER_CTRL_REG       0x4034
#define ANTENNA_DIV_CTRL_REG    0x4038

typedef enum {
    IEEE80211_NONE,
    IEEE80211_11A,
    IEEE80211_11B,
    IEEE80211_11G,
    IEEE80211_11N,
    IEEE80211_11AC,
    IEEE80211_11AX,
    IEEE80211_MAX
} ieee80211_standard_t;

typedef struct {
    ieee80211_standard_t standard;
    int is_enabled;
    void (*initialize)();
    void (*shutdown)();
} ieee80211_config_t;

void write_register(uint32_t address, uint32_t value) {
    *(volatile uint32_t *)address = value;
}

void set_tx_frame_control(uint32_t value) {
    write_register(TX_FRAME_CONTROL_REG, value);
}

void set_rx_frame_control(uint32_t value) {
    write_register(RX_FRAME_CONTROL_REG, value);
}

void set_mac_address(uint64_t address) {
    write_register(MAC_ADDR_REG, (uint32_t)(address & 0xFFFFFFFF));
    write_register(MAC_ADDR_REG + 4, (uint32_t)(address >> 32));
}

void set_channel(uint8_t channel) {
    write_register(CHANNEL_CTRL_REG, channel);
}

void set_data_rate(uint32_t rate) {
    write_register(DATA_RATE_REG, rate);
}

void set_power_management(uint8_t enable) {
    write_register(POWER_MGMT_REG, enable ? 1 : 0);
}

void ieee80211_init_11a() {
    set_tx_frame_control(0x12345678);
    set_rx_frame_control(0x87654321);
    set_mac_address(0xAABBCCDDEEFF);
    set_channel(36);
    set_data_rate(54000);
    set_power_management(1);
}

void ieee80211_init_11b() {
    set_tx_frame_control(0x11223344);
    set_rx_frame_control(0x44332211);
    set_mac_address(0x112233445566);
    set_channel(11);
    set_data_rate(11000);
    set_power_management(0);
}

void ieee80211_init_11g() {
    set_tx_frame_control(0x22334455);
    set_rx_frame_control(0x55443322);
    set_mac_address(0x665544332211);
    set_channel(6);
    set_data_rate(54000);
    set_power_management(1);
}

void ieee80211_init_11n() {
    set_tx_frame_control(0x33445566);
    set_rx_frame_control(0x66554433);
    set_mac_address(0x778899AABBCC);
    set_channel(11);
    set_data_rate(150000);
    set_power_management(1);
}

void ieee80211_init_11ac() {
    set_tx_frame_control(0x55667788);
    set_rx_frame_control(0x88776655);
    set_mac_address(0xA1B2C3D4E5F6);
    set_channel(36);
    set_data_rate(1300000);
    set_power_management(0);
}

void ieee80211_init_11ax() {
    set_tx_frame_control(0x66778899);
    set_rx_frame_control(0x99887766);
    set_mac_address(0xC1D2E3F4A5B6);
    set_channel(100);
    set_data_rate(2400000);
    set_power_management(1);
}

void ieee80211_configure(ieee80211_config_t *config) {
    if (config->is_enabled) {
        switch (config->standard) {
            case IEEE80211_11A:
                ieee80211_init_11a();
                break;
            case IEEE80211_11B:
                ieee80211_init_11b();
                break;
            case IEEE80211_11G:
                ieee80211_init_11g();
                break;
            case IEEE80211_11N:
                ieee80211_init_11n();
                break;
            case IEEE80211_11AC:
                ieee80211_init_11ac();
                break;
            case IEEE80211_11AX:
                ieee80211_init_11ax();
                break;
            default:
                break;
        }
    }
}

#endif // IEEE80211_H
