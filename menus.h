
#ifndef MENUS_H
#define MENUS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "game_fnc.h"
#include "lcd_text.h"
#include "colors.h"
#include "font_types.h"
#include "utils.h"

#define SPEED_1 1
#define SPEED_2 2
#define SPEED_3 0.5

#define COLOR_1 PINK_COLOR
#define COLOR_2 GREEN_COLOR
#define COLOR_3 RED_COLOR


typedef struct
{
    float paddle_speed;
    float ball_speed;
    unsigned int player_one_color;
    unsigned int player_two_color;
}settings_struct;

/*
    * opens menu with settings
*/
settings_struct settings_menu(unsigned char *mem_base, unsigned char *parlcd_mem_base, unsigned short *fb);

/*
    * cycles through diferent speed settings
    * speed - speed that is set right now
*/
float cycle_speed(float speed);

/*
    * cycles through diferent color settings
    * color - color that is set right now
*/
unsigned int cycle_colors(unsigned int color);

/*
    * opens screen that asks if you wanna play again or not
*/
bool play_again_screen(unsigned char *mem_base, unsigned char *parlcd_mem_base, unsigned short *fb);

#endif
