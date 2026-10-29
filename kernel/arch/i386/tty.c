#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <kernel/tty.h>
#include "kernel/vga.h"

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

int ansi_state = 0;
int ansi_value = 0;


size_t strlen(const char* str) 
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_MEMORY  0xB8000 

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer = (uint16_t*)VGA_MEMORY;

void terminal_initialize(void) 
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color) 
{
	terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_scroll(void) {
    /* Move every row up by one */
    /* The VGA hardware sees a flat line of 80 * 25 memory slots. Therefore, in order to get a location on thi       s 1D memory space, we need height and width to get a coordinate on the 2D visual screen coordinate */ 
    for (size_t y = 1; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t current_index  = y * VGA_WIDTH + x;
            const size_t above_index = (y - 1) * VGA_WIDTH + x;
            terminal_buffer[above_index] = terminal_buffer[current_index];
        }
    }

    for (size_t x = 0; x < VGA_WIDTH; x++) {
        const size_t bottom_index = (VGA_HEIGHT - 1) * VGA_WIDTH + x;
        terminal_buffer[bottom_index] = vga_entry(' ', terminal_color);
    }
}


void terminal_putchar(char c) 
{   
  if (ansi_state == 0) {
      if (c == '\033') {ansi_state = 1; return;}
        
      if (c == '\n') {
          terminal_column = 0;
          if (++terminal_row == 25) {
              terminal_scroll();
              terminal_row = VGA_HEIGHT - 1;
          }
          return ;
      }

	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT) {
            terminal_scroll();
			terminal_row = 24;
        }
  	}
  }
  /* State 1: Saw ESC */
  else if (ansi_state == 1) {
      if (c == '[') { ansi_state = 2; ansi_value = 0; return; } /* return after '[' */
      else {ansi_state = 0;}
  }
  /* State 2: REading command and numbers */
  if (ansi_state == 2) {
      if (c >= '0' && c <= '9') {
          ansi_value = ansi_value * 10 + (c - '0');
      }
      else if (c == 'm') {
          /* Color command */
          if (ansi_value == 0) {
                terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
          } else if (ansi_value >= 30 && ansi_value <= 37) {
                const uint8_t ansi_to_vga[] = {0, 4, 2, 6, 1, 5, 3, 7};
                terminal_setcolor(vga_entry_color(ansi_to_vga[ansi_value - 30], VGA_COLOR_BLACK));
          } else if (ansi_value >= 90 && ansi_value <= 97) {
                const uint8_t ansi_to_vga_bright[] = {8, 12, 10, 14, 9, 13, 11, 15};
                terminal_setcolor(vga_entry_color(ansi_to_vga_bright[ansi_value - 90], VGA_COLOR_BLACK));
          }
          ansi_state = 0;
      }
      else if (c == 'J') {
            /* Clear Screen Command */
            if (ansi_value == 2) {
                for (size_t y = 0; y < VGA_HEIGHT; y++) {
                    for (size_t x = 0; x < VGA_WIDTH; x++) {
                        terminal_putentryat(' ', terminal_color, x, y);
                    }
                }
                terminal_column = 0;
                terminal_row = 0;
            }
            ansi_state = 0;
        }
        else {
            ansi_state = 0; /* Unknown command, abort */
        }
  }
}


void terminal_write(const char* data, size_t size) 
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data) 
{
	terminal_write(data, strlen(data));
}

void terminal_write_status(const char* tag, uint8_t color) {
    terminal_writestring("[ ");
    /* Save current normal text color to memory */
    uint8_t old_color = terminal_color;
    /* Choosing a color */
    terminal_setcolor(vga_entry_color(color, VGA_COLOR_BLACK));
    
    terminal_writestring(tag);
    terminal_setcolor(old_color);
    terminal_writestring(" ]");
}

