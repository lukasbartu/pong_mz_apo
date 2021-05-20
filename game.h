#ifndef GAME_H
#define GAME_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include "game_fnc.h"
#include "menus.h"
#include "utils.h"
#include "lcd_text.h"

#define DISPLAY_WIDTH 480
#define DISPLAY_HEIGHT 320

#define PADDLE_HEIGHT 70

void keyboard_game(unsigned char *mem_base, unsigned char *parlcd_mem_base, unsigned short *fb, settings_struct *settings);
void knob_game(unsigned char *mem_base, unsigned char *parlcd_mem_base, unsigned short *fb, settings_struct *settings);

#endif
