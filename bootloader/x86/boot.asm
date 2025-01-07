; boot.asm
[BITS 16]
[ORG 0x7C00]   ; Load address is 0x7C00 (the location where BIOS loads the bootloader)

start:
    ; Initialize segments
    xor ax, ax             ; Clear the ax register
    mov ds, ax             ; Set the data segment to 0
    mov es, ax             ; Set the extra segment to 0
    mov ss, ax             ; Set the stack segment to 0
    mov sp, 0x7C00         ; Stack will be placed just after the bootloader

    ; Print the "Loading kernel..." message
    mov si, message
    call print_string

    ; Read the kernel into memory at address 0x10000 (Sector 2, 1 sector)
    mov ah, 0x02           ; BIOS: Read sector
    mov al, 1              ; Read 1 sector
    mov ch, 0              ; Track 0
    mov cl, 2              ; Sector 2
    mov dh, 0              ; Head 0
    int 0x13               ; BIOS interrupt to read the disk

    ; Check for disk read error
    jc disk_error

    ; Jump to the kernel address (0x10000)
    jmp 0x1000:0x0000      ; Jump to 0x10000 (kernel load address)

disk_error:
    ; Handle disk read error
    mov si, error_msg
    call print_string
    hlt                    ; Halt the CPU

print_string:
    ; Print the string passed in si
    mov ah, 0x0E
.next_char:
    lodsb
    or al, al
    jz .done
    int 0x10               ; BIOS interrupt to print the character
    jmp .next_char
.done:
    ret

message: db "Loading kernel...", 0
error_msg: db "Disk read error!", 0

times 510 - ($ - $$) db 0  ; Fill the remaining space with zeros up to 510 bytes
dw 0xAA55                 ; Bootloader magic number
