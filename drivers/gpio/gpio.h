#ifndef GPIO_H
#define GPIO_H

// Base address for GPIO controller (Raspberry Pi)
#define GPIO_BASE_ADDR 0x20200000

// Registers for controlling GPIO pins
#define GPFSEL0   (*(volatile unsigned int *)(GPIO_BASE_ADDR + 0x00))  // 0x00 - GPIO Function Select 0
#define GPSET0    (*(volatile unsigned int *)(GPIO_BASE_ADDR + 0x1C))  // 0x1C - GPIO Pin Output Set 0
#define GPCLR0    (*(volatile unsigned int *)(GPIO_BASE_ADDR + 0x28))  // 0x28 - GPIO Pin Output Clear 0
#define GPLEV0    (*(volatile unsigned int *)(GPIO_BASE_ADDR + 0x34))  // 0x34 - GPIO Pin Level 0

// Macro for generating the mask for a specific pin
#define GPIO_PIN_MASK(pin) (1 << (pin))

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
