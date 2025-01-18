ENTRY(i2c_main)
SECTIONS {
    . = 0x10000;  
    .text : {
        *(.text) 
    }
    .data : {
        *(.data)
    }
}
