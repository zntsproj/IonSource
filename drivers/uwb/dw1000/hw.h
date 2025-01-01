// UWB Hardware registers.
// Created by: MAIN developer of ION Kernel
// FOR DW1000 CONTROLLER!

#ifndef UWB_HW_H
#define UWB_HW_H

// Register addresses for DW1000 UWB controller
#define PANADR      0x03  // PAN address register
#define SYS_CFG     0x04  // System configuration register
#define SYS_TIME    0x06  // System time counter
#define TX_FCTRL    0x08  // Transmit frame control
#define DX_TIME     0x0A  // Delayed transmit time
#define RX_FCTRL    0x0C  // Receive frame control
#define SYS_STATUS  0x0F  // System event status register
#define RX_FINFO    0x10  // Receive frame information
#define RX_BUFFER   0x11  // Receive buffer start
#define TX_BUFFER   0x12  // Transmit buffer start
#define PMSC_CTRL0  0x36  // Power management system control 0

#endif // UWB_HW_H
