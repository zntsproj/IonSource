#ifndef GPIO_H
#define GPIO_H

// Base address for GPIO controller
#define GPIO_BASE_ADDR 0x20200000

// Registers for controlling GPIO pins (each GPFSELx controls 10 pins)
#define GPFSEL0   (*(volatile unsigned int *)(GPIO_BASE_ADDR + 0x00))  // 0x00 - GPIO Function Select 0 (pins 0-9)
#define GPFSEL1   (*(volatile unsigned int *)(GPIO_BASE_ADDR + 0x04))  // 0x04 - GPIO Function Select 1 (pins 10-19)
#define GPFSEL2   (*(volatile unsigned int *)(GPIO_BASE_ADDR + 0x08))  // 0x08 - GPIO Function Select 2 (pins 20-29)
#define GPFSEL3   (*(volatile unsigned int *)(GPIO_BASE_ADDR + 0x0C))  // 0x0C - GPIO Function Select 3 (pins 30-39)
#define GPFSEL4   (*(volatile unsigned int *)(GPIO_BASE_ADDR + 0x10))  // 0x10 - GPIO Function Select 4 (pins 40-49)
#define GPFSEL5   (*(volatile unsigned int *)(GPIO_BASE_ADDR + 0x14))  // 0x14 - GPIO Function Select 5 (pins 50-53)

// Registers for setting and clearing GPIO pins
#define GPSET0    (*(volatile unsigned int *)(GPIO_BASE_ADDR + 0x1C))  // 0x1C - GPIO Pin Output Set 0
#define GPSET1    (*(volatile unsigned int *)(GPIO_BASE_ADDR + 0x20))  // 0x20 - GPIO Pin Output Set 1 (pins 32-53)
#define GPCLR0    (*(volatile unsigned int *)(GPIO_BASE_ADDR + 0x28))  // 0x28 - GPIO Pin Output Clear 0
#define GPCLR1    (*(volatile unsigned int *)(GPIO_BASE_ADDR + 0x2C))  // 0x2C - GPIO Pin Output Clear 1 (pins 32-53)
#define GPLEV0    (*(volatile unsigned int *)(GPIO_BASE_ADDR + 0x34))  // 0x34 - GPIO Pin Level 0
#define GPLEV1    (*(volatile unsigned int *)(GPIO_BASE_ADDR + 0x38))  // 0x38 - GPIO Pin Level 1 (pins 32-53)

// Macro for generating the mask for a specific pin
#define GPIO_PIN_MASK(pin) (1 << ((pin) % 32))

// Function declarations for GPIO management

// Initializes the GPIO pins, setting them to default configurations
void gpio_init(void);

// Sets the direction of a specific pin: 1 for output, 0 for input
void gpio_set_pin_direction(unsigned int pin, unsigned int direction);

// Writes a value to a specific pin: 1 for high, 0 for low
void gpio_write_pin(unsigned int pin, unsigned int value);

// Reads the value of a specific pin: returns 1 for high, 0 for low
unsigned int gpio_read_pin(unsigned int pin);

#endif // GPIO_H
