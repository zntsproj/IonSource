#include "rf_modem.h"
#include "ieee80222.h"
#include <string.h>
#include <stdio.h>

struct wran_dev {
    struct rf_module_device rf_device;
    ieee80222_base_station_t base_station;
};

static inline void wran_dev_init(struct wran_dev *dev, uint32_t base_station_id, uint32_t frequency) {
    rf_module_init(&dev->rf_device);
    rf_module_irq_register(&dev->rf_device);
    ieee80222_create_base_station(base_station_id, frequency);
    printf("WRAN device initialized\n");
}

static inline void wran_dev_add_station(struct wran_dev *dev, uint32_t station_id, uint32_t address) {
    ieee80222_add_station(dev->base_station.base_station_id, station_id, address);
    printf("Station added: %u\n", station_id);
}

static inline void wran_dev_send_data(struct wran_dev *dev, uint32_t source, uint32_t destination, uint8_t *data, size_t data_len) {
    ieee80222_send_data(source, destination, data, data_len);
    printf("Data sent: %zu bytes\n", data_len);
}

static inline void wran_dev_receive_data(struct wran_dev *dev, uint8_t *buffer, size_t buffer_len) {
    ieee80222_receive_data(buffer, buffer_len);
    printf("Data received: %zu bytes\n", buffer_len);
}

void wran_dev_rf_irq_handler(void *data) {
    struct rf_module_device *rf_device = (struct rf_module_device *)data;
    printf("IRQ handled: mode %d\n", rf_device->mode);
}

static inline void wran_dev_start(struct wran_dev *dev) {
    rf_module_set_mode(&dev->rf_device, RF_MODE_RX);  // Use RF_MODE_RX for receive mode
    printf("WRAN device started in RX mode\n");
}

int main() {
    struct wran_dev wran_device;

    wran_dev_init(&wran_device, 12345, 700e6);
    wran_dev_add_station(&wran_device, 1, 0xAABBCCDD);

    uint8_t data[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB};
    wran_dev_send_data(&wran_device, 1, 2, data, sizeof(data));

    uint8_t buffer[2048];
    wran_dev_receive_data(&wran_device, buffer, sizeof(buffer));

    wran_dev_start(&wran_device);
    wran_dev_rf_irq_handler(&wran_device.rf_device);

    return 0;
}
