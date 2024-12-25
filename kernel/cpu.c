#include <stdint.h>   // For uint64_t types

#define CR0_CACHE_ENABLE 0x00000010
#define CR0_USER_MODE    0x00000003

#define KERNEL_MODE 0   // Define kernel mode (can be any value, here 0)
#define USER_MODE   1   // Define user mode (optional, for clarity)

/* Function declarations */
void enable_cache(void);
void enable_interrupts(void);
void set_cpu_mode(int mode);
uint64_t read_cr0(void);
void write_cr0(uint64_t value);
void* idt_ptr = (void*)0x1200000;  // IDT base address (adjust as needed)

/* Function to initialize the CPU */
void init_cpu(void) {
    enable_cache();
    enable_interrupts();
    set_cpu_mode(KERNEL_MODE);
}

/* Function to enable the CPU cache */
void enable_cache(void) {
    // Enable the CPU cache by setting the CR0 register (x86-64 specific)
    uint64_t cr0 = read_cr0();
    cr0 |= CR0_CACHE_ENABLE;
    write_cr0(cr0);
}

/* Function to enable interrupts (IDT setup) */
void enable_interrupts(void) {
    // Set up the Interrupt Descriptor Table (IDT) for handling interrupts
    // Set up the PIC (Programmable Interrupt Controller) for x86-64 interrupt handling
    __asm__ __volatile__ (
        "lidt %0"   // Load IDT, where idt_ptr is a 64-bit pointer to the IDT descriptor
        : 
        : "m"(idt_ptr)  // Use the 'm' constraint to load memory address of idt_ptr
    );
    // The IDT descriptor is loaded with the address stored in idt_ptr
}

/* Function to set the CPU mode (user/kernel mode) */
void set_cpu_mode(int mode) {
    uint64_t cr0 = read_cr0();

    if (mode == KERNEL_MODE) {
        cr0 &= ~CR0_USER_MODE;  // Switch to kernel mode (disable user mode)
    } else {
        cr0 |= CR0_USER_MODE;   // Switch to user mode (enable user mode)
    }

    write_cr0(cr0);
}

/* Helper functions to interact with CPU registers (CR0) */
uint64_t read_cr0(void) {
    uint64_t cr0;
    __asm__ __volatile__("mov %%cr0, %0" : "=r"(cr0));
    return cr0;
}

void write_cr0(uint64_t value) {
    __asm__ __volatile__("mov %0, %%cr0" : : "r"(value));
}
