#include "spi.h"
#include "stdio.h"
#include "io_dma.h"
#include "rf_modem.h"

int dwuwb_init () {
    printf("UWB: Initializing UWB Device via SPI.\n");
    
    uint8_t data[4];
    
    uintptr_t base_address = 0x1000;
    if (!check_hardware(base_address)) {
        printf("Error: UWB hardware not found at base address 0x%lx.\n", base_address);
        return -1;
    }

    data[0] = 0x01;
    spi_transfer(SPI_CS_PIN, data, 1);

    data[0] = 0x02;
    spi_transfer(SPI_CS_PIN, data, 1);

    struct rf_module_device rf_device;
    rf_module_init(&rf_device);
    rf_module_set_mode(&rf_device, RF_MODE_RX);
    rf_module_irq_register(&rf_device);

    printf("UWB: Device initialized successfully via SPI.\n");
    return 0;
}
