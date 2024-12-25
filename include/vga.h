#ifndef VGA_H
#define VGA_H

#define VGA_CTRL_PORT  0x3D4
#define VGA_DATA_PORT  0x3D5

#define VGA_COLOR_BLACK    0
#define VGA_COLOR_BLUE     1
#define VGA_COLOR_GREEN    2
#define VGA_COLOR_CYAN     3
#define VGA_COLOR_RED      4
#define VGA_COLOR_MAGENTA  5
#define VGA_COLOR_BROWN    6
#define VGA_COLOR_LIGHT_GREY 7
#define VGA_COLOR_DARK_GREY 8
#define VGA_COLOR_LIGHT_BLUE 9
#define VGA_COLOR_LIGHT_GREEN 10
#define VGA_COLOR_LIGHT_CYAN 11
#define VGA_COLOR_LIGHT_RED 12
#define VGA_COLOR_LIGHT_MAGENTA 13
#define VGA_COLOR_LIGHT_BROWN 14
#define VGA_COLOR_WHITE 15

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

typedef struct {
    unsigned char character;
    unsigned char color;
} vga_char;

volatile vga_char* vga_buffer = (volatile vga_char*) 0xB8000;

int cursor_x = 0;
int cursor_y = 0;

// Объявление функции перед её использованием
void vga_clear_screen();

void vga_init() {
    vga_clear_screen();
}

void vga_clear_screen() {
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            vga_buffer[y * VGA_WIDTH + x].character = ' ';
            vga_buffer[y * VGA_WIDTH + x].color = VGA_COLOR_BLACK;
        }
    }
    cursor_x = cursor_y = 0;
}

void vga_print_char(char c, unsigned char color) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else {
        vga_buffer[cursor_y * VGA_WIDTH + cursor_x].character = c;
        vga_buffer[cursor_y * VGA_WIDTH + cursor_x].color = color;
        cursor_x++;
    }
    
    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }
    
    if (cursor_y >= VGA_HEIGHT) {
        cursor_y = 0;
    }
}

void vga_print_string(const char* str, unsigned char color) {
    while (*str) {
        vga_print_char(*str, color);
        str++;
    }
}

#endif // VGA_H
