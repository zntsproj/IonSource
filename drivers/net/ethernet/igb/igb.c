#include "io.h"

#define IGB_BASE_ADDR 0x0000  // Base address for the registers

#define GT_RESET_REG     (IGB_BASE_ADDR + 0x0000)
#define RESET_REG        (IGB_BASE_ADDR + 0x0004)
#define MODE_REG         (IGB_BASE_ADDR + 0x0008)
#define CONFIGURATION_TX_REG1   (IGB_BASE_ADDR + 0x000C)
#define CONFIGURATION_RX_REG1   (IGB_BASE_ADDR + 0x0014)

typedef struct {
    uint32_t mode;
    uint32_t tx_config;
    uint32_t rx_config;
} igb_device_t;

void igb_reset_device(igb_device_t *dev) {
    outl(GT_RESET_REG, 1);  
    io_wait();  
    outl(GT_RESET_REG, 0);  
    io_wait();  
}

void igb_init_mode(igb_device_t *dev) {
    outl(MODE_REG, dev->mode);
}

void igb_configure_tx(igb_device_t *dev) {
    outl(CONFIGURATION_TX_REG1, dev->tx_config);
}

void igb_configure_rx(igb_device_t *dev) {
    outl(CONFIGURATION_RX_REG1, dev->rx_config);
}

void igb_init(igb_device_t *dev) {
    igb_reset_device(dev);
    igb_init_mode(dev);
    igb_configure_tx(dev);
    igb_configure_rx(dev);
}

void igb_transmit_packet(uint8_t *packet, size_t length) {
    uint16_t tx_port = 0x0008;  
    outb_buffer(tx_port, packet, length);
}

void igb_receive_packet(uint8_t *buffer, size_t buffer_size) {
    uint16_t rx_port = 0x0010;  
    inb_buffer(rx_port, buffer, buffer_size);
}
