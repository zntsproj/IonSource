/* ARM Bootloader */
.section .text
.global _start

/* Entry point of the bootloader */
_start:
    /* Disable interrupts */
    cpsid i

    /* Setup the stack pointer */
    ldr sp, =stack_top

    /* Initialize UART for serial communication */
    bl uart_init

    /* Print boot message */
    ldr r0, =boot_msg
    bl uart_print_string

    /* Load kernel into memory */
    ldr r0, =kernel_address  /* Destination address for the kernel */
    ldr r1, =kernel_source   /* Source address or device */
    ldr r2, =kernel_size     /* Size of the kernel in bytes */
    bl load_kernel

    /* Jump to the loaded kernel */
    ldr r0, =kernel_address
    bx  r0

/* Infinite loop in case of error */
error:
    b error

/* UART Initialization */
uart_init:
    /* UART base address is 0x09000000 */
    ldr r0, =0x09000000      /* UART base */
    mov r1, #0x03            /* Control: Enable TX, RX */
    str r1, [r0]
    bx lr

/* Print a null-terminated string using UART */
uart_print_string:
    ldr r1, =0x09000000      /* UART base */
.next_char:
    ldrb r2, [r0], #1        /* Load next character */
    cmp r2, #0               /* Check for null terminator */
    beq .done
    strb r2, [r1]            /* Send character to UART */
    b .next_char
.done:
    bx lr

/* Load kernel into memory (simplified example) */
load_kernel:
    /* Stub implementation - copy from memory for now */
    ldr r3, =0  /* Placeholder: Replace with actual device logic */
.copy_loop:
    ldrb r4, [r1], #1  /* Read byte */
    strb r4, [r0], #1  /* Write byte */
    subs r2, r2, #1    /* Decrement size */
    bne .copy_loop
    bx lr

/* Memory layout */
.section .data
boot_msg: .asciz "Booting ION Kernel...\n"
kernel_address: .word 0x10000       /* Address where kernel will be loaded */
kernel_source:  .word 0x100000     /* Source address (e.g., from storage) */
kernel_size:    .word 4096         /* Size in bytes */
stack_top:      .word stack + 1024 /* Address of stack top */

/* Stack setup */
.section .bss
.balign 8
stack: .space 1024
