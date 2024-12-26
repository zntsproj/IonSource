#include "gpio.h"

// compile: gcc gpio.c -nostdlib

/**
 * gpio_init - Initializes GPIO by setting the first 10 pins to output by default.
 * This function configures the initial setup for the GPIO pins.
 */
void gpio_init(void) {
    // Set the first 10 pins as output
    GPFSEL0 = 0x55555555;  // Set pins 0-9 to output (111 for output mode in the GPFSEL0 register)
}

/**
 * gpio_set_pin_direction - Configures a GPIO pin as input or output.
 * @pin: The GPIO pin number (0-53).
 * @direction: 1 for output, 0 for input.
 * 
 * This function sets the direction of the specified GPIO pin.
 */
void gpio_set_pin_direction(unsigned int pin, unsigned int direction) {
    unsigned int register_index = pin / 10;  // Each register controls 10 pins (0-9, 10-19, etc.)
    unsigned int shift = (pin % 10) * 3;     // Each pin is controlled by 3 bits in the register
    unsigned int mask = ~(0b111 << shift);   // Mask to clear the current pin configuration

    // Pointer to the correct function select register
    volatile unsigned int *reg;
    if (register_index == 0) reg = &GPFSEL0;
    else if (register_index == 1) reg = &GPFSEL1;
    else if (register_index == 2) reg = &GPFSEL2;
    else if (register_index == 3) reg = &GPFSEL3;
    else if (register_index == 4) reg = &GPFSEL4;
    else if (register_index == 5) reg = &GPFSEL5;
    else return;  // If the pin index is invalid, exit

    *reg &= mask;  // Clear the bits for the pin
    *reg |= (direction << shift);  // Set the pin direction (0 for input, 1 for output)
}

/**
 * gpio_write_pin - Sets the value of a GPIO pin (high or low).
 * @pin: The GPIO pin number (0-53).
 * @value: 1 to set the pin high, 0 to set the pin low.
 * 
 * This function sets the specified GPIO pin to the given value.
 */
void gpio_write_pin(unsigned int pin, unsigned int value) {
    if (value) {
        GPSET0 = GPIO_PIN_MASK(pin);  // Set the pin high (1)
    } else {
        GPCLR0 = GPIO_PIN_MASK(pin);  // Set the pin low (0)
    }
}

/**
 * gpio_read_pin - Reads the current value of a GPIO pin.
 * @pin: The GPIO pin number (0-53).
 * 
 * This function returns the current level of the specified GPIO pin.
 * It returns 1 if the pin is high, and 0 if the pin is low.
 */
unsigned int gpio_read_pin(unsigned int pin) {
    return (GPLEV0 & GPIO_PIN_MASK(pin)) != 0;  // Return 1 if the pin is high, 0 if it is low
}
