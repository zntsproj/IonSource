#include "kernel.h"  // Kernel header from includes
#include "hw.h"      // Hardware access helpers (hw_read_reg, hw_write_reg)
#include <stdint.h>
#include <string.h>

#define ATHEROS_DEVICE_ID 0x003C
#define ATHEROS_VENDOR_ID 0x168C

#define ATHEROS_REG_CTRL       0x00
#define ATHEROS_REG_STATUS     0x04
#define ATHEROS_REG_TX_CTRL    0x08
#define ATHEROS_REG_RX_CTRL    0x0C
#define ATHEROS_REG_RX_BUFFER  0x10
#define ATHEROS_REG_TX_BUFFER  0x14

struct atheros_device {
    uintptr_t base_addr;
    uint32_t irq;
    uint32_t rx_buffer[256];
    uint32_t tx_buffer[256];
};

static struct atheros_device ath_dev;

static int atheros_init(uintptr_t base_addr, uint32_t irq) {
    ath_dev.base_addr = base_addr;
    ath_dev.irq = irq;

    hw_write_reg(ath_dev.base_addr, ATHEROS_REG_CTRL, 0x1);
    while (hw_read_reg(ath_dev.base_addr, ATHEROS_REG_STATUS) & 0x1) {
    }

    printk("[Atheros] Device initialized at base address 0x%lx with IRQ %d\n", base_addr, irq);
    return 0;
}

static void atheros_start(void) {
    hw_write_reg(ath_dev.base_addr, ATHEROS_REG_RX_CTRL, 0x1);
    hw_write_reg(ath_dev.base_addr, ATHEROS_REG_TX_CTRL, 0x1);
    printk("[Atheros] Device started. RX and TX enabled.\n");
}

static void atheros_stop(void) {
    hw_write_reg(ath_dev.base_addr, ATHEROS_REG_RX_CTRL, 0x0);
    hw_write_reg(ath_dev.base_addr, ATHEROS_REG_TX_CTRL, 0x0);
    printk("[Atheros] Device stopped. RX and TX disabled.\n");
}

static void atheros_isr(void) {
    uint32_t status = hw_read_reg(ath_dev.base_addr, ATHEROS_REG_STATUS);

    if (status & 0x01) {
        uint32_t *rx_data = ath_dev.rx_buffer;
        printk("[Atheros] Data received: %d bytes\n", sizeof(ath_dev.rx_buffer));
    }

    if (status & 0x02) {
        memset(ath_dev.tx_buffer, 0, sizeof(ath_dev.tx_buffer));
        printk("[Atheros] Data sent, TX buffer cleared.\n");
    }

    hw_write_reg(ath_dev.base_addr, ATHEROS_REG_STATUS, status);
}

static int atheros_send_data(const uint8_t *data, size_t length) {
    if (length > sizeof(ath_dev.tx_buffer)) {
        return -1;
    }

    memcpy(ath_dev.tx_buffer, data, length);
    hw_write_reg(ath_dev.base_addr, ATHEROS_REG_TX_BUFFER, (uintptr_t)ath_dev.tx_buffer);

    return 0;
}

static int atheros_receive_data(uint8_t *data, size_t max_length) {
    size_t length = sizeof(ath_dev.rx_buffer);
    if (length > max_length) {
        return -1;
    }

    memcpy(data, ath_dev.rx_buffer, length);
    return length;
}

int atheros_driver_entry(uintptr_t base_addr, uint32_t irq) {
    int ret = atheros_init(base_addr, irq);
    if (ret != 0) {
        printk("[Atheros] Initialization failed\n");
        return ret;
    }

    atheros_start();
    printk("[Atheros] Driver loaded successfully\n");
    return 0;
}

void atheros_driver_exit(void) {
    atheros_stop();
    printk("[Atheros] Driver unloaded successfully\n");
}

#ifdef TEST_DRIVER
int main(void) {
    uintptr_t base_addr = 0x00080000;
    uint32_t irq = 5;

    atheros_driver_entry(base_addr, irq);

    atheros_isr();

    uint8_t data_to_send[256] = { 0x01, 0x02, 0x03, 0x04 };
    atheros_send_data(data_to_send, sizeof(data_to_send));

    uint8_t received_data[256];
    int received_length = atheros_receive_data(received_data, sizeof(received_data));
    if (received_length > 0) {
        printk("[Atheros] Received %d bytes\n", received_length);
    }

    atheros_driver_exit();
    return 0;
}
#endif
