[BITS 16]
[ORG 0x7C00]

start:
    cli                    
    xor ax, ax
    mov ds, ax             

    ; Сообщение о запуске
    mov si, boot_msg
    call print_string

    ; Инициализация загрузки ядра
    mov ax, 0x1000          ; segment address
    mov es, ax             
    xor bx, bx              

    mov ah, 0x02            ; BIOS: read sectors
    mov al, 1               ; 1 sector
    mov ch, 0               ; road 0
    mov cl, 2               ; sector 2
    mov dh, 0         
    int 0x13           

    jc disk_error      
    jmp 0x10000       

disk_error:
    mov si, error_msg
    call print_string
    cli
    hlt

print_string:
    mov ah, 0x0E     
.next_char:
    lodsb                  
    or al, al         
    jz .done
    int 0x10
    jmp .next_char
.done:
    ret

boot_msg db "Booting ION kernel...", 0
error_msg db "Disk read error!", 0

times 510-($-$$) db 0
dw 0xAA55                 
