#include "gpio.h"

// compile: gcc gpio.c -nostdlib

/**
 * GPIO base addresses and helper macros.
 */
#define GPIO_PIN_COUNT 54             // Total number of GPIO pins
#define GPIO_FUNC_BITS 3              // Each pin uses 3 bits in the function select register
#define GPIO_FUNC_MASK 0b111          // Mask for a single pin's function bits
#define GPIO_PIN_MASK(pin) (1U << ((pin) % 32))

/**
 * gpio_init - Initializes GPIO by setting the first 10 pins to output by default.
 * This function configures the initial setup for the GPIO pins.
 */
void gpio_init(void) {
    // Check if GPFSEL0 address is valid (optional: depends on platform)
    if (!&GPFSEL0) return;  // Prevent segfault if GPFSEL0 is not mapped

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
    // Validate pin number
    if (pin >= GPIO_PIN_COUNT) return;

    unsigned int register_index = pin / 10;  // Each register controls 10 pins (0-9, 10-19, etc.)
    unsigned int shift = (pin % 10) * GPIO_FUNC_BITS;  // Each pin is controlled by 3 bits in the register
    unsigned int mask = ~(GPIO_FUNC_MASK << shift);   // Mask to clear the current pin configuration

    // Pointer to the correct function select register
    volatile unsigned int *reg;
    switch (register_index) {
        case 0: reg = &GPFSEL0; break;
        case 1: reg = &GPFSEL1; break;
        case 2: reg = &GPFSEL2; break;
        case 3: reg = &GPFSEL3; break;
        case 4: reg = &GPFSEL4; break;
        case 5: reg = &GPFSEL5; break;
        default: return;  // Invalid register index
    }

    if (!reg) return;  // Prevent null pointer dereference

    *reg &= mask;  // Clear the bits for the pin
    *reg |= ((direction ? 1 : 0) << shift);  // Set the pin direction (0 for input, 1 for output)
}

/**
 * gpio_write_pin - Sets the value of a GPIO pin (high or low).
 * @pin: The GPIO pin number (0-53).
 * @value: 1 to set the pin high, 0 to set the pin low.
 *
 * This function sets the specified GPIO pin to the given value.
 */
void gpio_write_pin(unsigned int pin, unsigned int value) {
    // Validate pin number
    if (pin >= GPIO_PIN_COUNT) return;

    volatile unsigned int *set_reg = (pin < 32) ? &GPSET0 : &GPSET1;
    volatile unsigned int *clr_reg = (pin < 32) ? &GPCLR0 : &GPCLR1;

    if (value) {
        if (set_reg) *set_reg = GPIO_PIN_MASK(pin);  // Set the pin high (1)
    } else {
        if (clr_reg) *clr_reg = GPIO_PIN_MASK(pin);  // Set the pin low (0)
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
    // Validate pin number
    if (pin >= GPIO_PIN_COUNT) return 0;

    volatile unsigned int *lev_reg = (pin < 32) ? &GPLEV0 : &GPLEV1;
    if (!lev_reg) return 0;  // Prevent null pointer dereference

    return ((*lev_reg & GPIO_PIN_MASK(pin)) != 0) ? 1 : 0;
}
