#ifndef LCD_TEXT_H
#define LCD_TEXT_H

#include "font_types.h"

void draw_pixel(int x, int y, unsigned short color, unsigned short *fb);
void draw_char(int x, int y, font_descriptor_t *fdes, char ch, unsigned short *fb);
int char_width(font_descriptor_t *fdes, int ch);

#endif
