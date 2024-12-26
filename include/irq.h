#ifndef ION_IRQ_H
#define ION_IRQ_H

#include "io.h"  // For I/O port operations
#include "kernel.h"  // For printk

// Structure to describe an interrupt handler
struct irq_desc {
    unsigned int irq;               // Interrupt number
    void (*handler)(void *data);    // Interrupt handler function
    void *data;                     // Data that can be passed to the handler
    unsigned int priority;          // Priority of the interrupt
};

// Maximum number of interrupts
#define MAX_IRQS 256

// Interrupt descriptor table
static struct irq_desc irq_table[MAX_IRQS];

// Register an interrupt handler with priority
static inline void irq_set_handler(unsigned int irq, void (*handler)(void *), void *data, unsigned int priority) {
    if (irq >= MAX_IRQS) {
        printk("Error: IRQ number out of range!\n");
        return;
    }
    irq_table[irq].irq = irq;
    irq_table[irq].handler = handler;
    irq_table[irq].data = data;
    irq_table[irq].priority = priority;
    printk("Handler set for IRQ %u with priority %u\n", irq, priority);
}

// Remove an interrupt handler
static inline void irq_remove_handler(unsigned int irq) {
    if (irq >= MAX_IRQS) {
        printk("Error: IRQ number out of range for removal!\n");
        return;
    }
    irq_table[irq].irq = 0;
    irq_table[irq].handler = NULL;
    irq_table[irq].data = NULL;
    irq_table[irq].priority = 0;
    printk("Handler removed for IRQ %u\n", irq);
}

// Handle an interrupt by calling its handler
static inline void irq_handle(unsigned int irq) {
    if (irq >= MAX_IRQS || irq_table[irq].handler == NULL) {
        printk("Error: No handler registered for IRQ %u\n", irq);
        return;
    }

    // Call the interrupt handler
    irq_table[irq].handler(irq_table[irq].data);
}

// Initialize the interrupt system
void irq_init(void) {
    // Clear the interrupt descriptor table
    for (unsigned int i = 0; i < MAX_IRQS; i++) {
        irq_table[i].irq = 0;
        irq_table[i].handler = NULL;
        irq_table[i].data = NULL;
        irq_table[i].priority = 0;
    }
    printk("IRQ system initialized\n");
}

// Enable an interrupt
static inline void irq_enable(unsigned int irq) {
    if (irq >= MAX_IRQS) {
        printk("Error: IRQ number out of range for enabling!\n");
        return;
    }

    // Use I/O port to enable interrupt (example for PIC with 0x21 port)
    unsigned char mask = inb(0x21);
    mask &= ~(0x01 << irq);  // Clear the bit to enable the IRQ
    outb(0x21, mask);

    printk("IRQ %u enabled\n", irq);
}

// Disable an interrupt
static inline void irq_disable(unsigned int irq) {
    if (irq >= MAX_IRQS) {
        printk("Error: IRQ number out of range for disabling!\n");
        return;
    }

    // Use I/O port to disable interrupt (example for PIC with 0x21 port)
    unsigned char mask = inb(0x21);
    mask |= (0x01 << irq);  // Set the bit to disable the IRQ
    outb(0x21, mask);

    printk("IRQ %u disabled\n", irq);
}

// Temporarily disable all interrupts and return the previous state
static inline unsigned int irq_save_disable(void) {
    unsigned int flags;
    asm volatile (
        "pushf\n\t"
        "cli\n\t"
        "pop %0"
        : "=r"(flags)
        :
        : "memory"
    );
    return flags;
}

// Restore the interrupt state
static inline void irq_restore(unsigned int flags) {
    asm volatile (
        "push %0\n\t"
        "popf"
        :
        : "r"(flags)
        : "memory", "cc"
    );
}

#endif // ION_IRQ_H
