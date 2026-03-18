#include "screen.h"
#include <stdarg.h>

static uint16_t* vga_buffer = (uint16_t*)0xB8000;
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;
static uint8_t current_color = 0x07;


void screen_init(void){
    clear_screen();
}

void clear_screen(void){
    for(int i = 0; i < 80 * 25; i++){
        vga_buffer[i]   = (uint16_t)0x0720;
    }
    cursor_x = 0;
    cursor_y = 0;
}

void set_color(uint8_t fg, uint8_t bg){
    current_color = (bg << 4) | (fg & 0x0F);
}

static void scroll(void){
    for(int i = 80 * 24; i < 80 * 25; i++){
        vga_buffer[i] = vga_buffer[i + 80];
    }
    for(int i = 80 * 24; i < 80 * 25; i++){
        vga_buffer[i] = (uint16_t)0x0720;
    }
    cursor_y = 24;
}

void putchar(char c){
    if(c == '\n'){
        cursor_x = 0;
        cursor_y++;
    }else if (c >= ' '){
        uint16_t entry = (current_color << 8) | c;
        vga_buffer[cursor_y * 80 + cursor_x] = entry;
        cursor_x++;
    }

    if(cursor_x >= 80){
        cursor_x = 0;
        cursor_y++;
    }
    if(cursor_y >= 25){
        scroll();
        cursor_y = 24;
    }
}

void puts(const char* str){
    while(*str) putchar(*str++);
}

void printf(const char* format, ...){
    va_list args;
    va_start(args, format);

    while(*format){
        if(*format == '%'){
            format++;
            if(*format == 's'){
                puts(va_arg(args, char*));
            } else if(*format == 'd'){
                int num = va_arg(args, int);
                if(num < 0) {putchar('-'); num = -num;}
                char buf[16]; int i = 0;
                do {buf[i++] = '0' + (num % 10); num /= 10;} while (num);
                while (i--) putchar(buf[i]);
            } else if(*format == 'x'){
                unsigned int num = va_arg(args, unsigned int);
                char buf[16]; int i = 0;
                do {buf[i++] = (num % 16 < 10) ? '0' + (num % 16) : 'a' + (num % 16 -10); num /= 16;} while (num);
                while (i--) putchar(buf[i]);
            } else if (*format == 'c'){
                putchar((char)va_arg(args, int));
            }
        } else {
            putchar(*format);
        }
        format++;
    }
    va_end(args);
}