#ifndef LCD_TEXT_H
#define LCD_TEXT_H

#include "font_types.h"
#include "mzapo_parlcd.h"
#include "lcd_text.h"
#include "colors.h"
#include <stdio.h>

/*
    * fills pixel with color
    * x,y - coords, color - color that will be use to fill pixel, fb - pointer on frame buffer
*/
void draw_pixel(int x, int y, unsigned short color, unsigned short *fb);

/*
    * draws a char out of the font library
    * x,y - coords, fdes - font descriptor, ch - char to be drawn, fb - pointer on frame buffer
*/
void draw_char(int x, int y, font_descriptor_t *fdes, char ch, unsigned int color, unsigned short *fb);

/*
    * returns pixel width of char
    * fdes - font descriptor, ch - tested char
*/
int char_width(font_descriptor_t *fdes, int ch);

/*
    * repeatedly calls draw_char to print the whole string
    * x,y - coords, length - length of string, fdes - font descriptor, fb - pointer on frame buffer
*/
void draw_string(int x, int y, char *string, int length, unsigned int color, font_descriptor_t *fdes, unsigned short *fb);

/*
    * redraws screen with buffer content
    * parlcd_mem_base - memory base, fb - pointer on frame buffer
*/
void lcd_draw(unsigned char *parlcd_mem_base, unsigned short *fb);

/*
    * clears frame buffer - makes every pixel black
    * fb - pointer on frame buffer
*/
void frame_buffer_clear(unsigned short *fb);
#endif
