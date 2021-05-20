#ifndef GAME_FNC_H
#define GAME_FNC_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>

#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "font_types.h"
#include "lcd_text.h"
#include "utils.h"


#define PADDLE_THICNESS 20 //px
#define PADDLE_HEIGHT 70   //px
#define KNOB_MAX_VALUE 0xFF //used to calculate relative position of knob


/*paddle struct - carries information about player's paddle*/
typedef struct
{
  int start;    //start value for frame buffer
  int thicness; //constant for frame buffer
  int offset;   //offset of position from top for frame buffer
  int position; //position of paddle
  int edge;
  float speed;
} paddle;

typedef struct {
  int pos_x;
  int pos_y;
  int start_x;
  int start_y;
  float dy;
  float dx;
  bool collided;
  int offset;
} ball;

typedef struct {
  int score_p1;
  int score_p2;
  
  int goal;
} game;

paddle initLeftpaddle();
paddle initRightpaddle();
ball init_ball();

void update_paddle_position(unsigned char *mem_base, paddle *left, paddle *right);

void draw_score(unsigned short *fb, unsigned char *parlcd_mem_base, game *game);
void goal(int p, unsigned char *mem_base, unsigned short *fb, unsigned char *parlcd_mem_base, game *game);

void update_ball(unsigned char *mem_base, ball *ball, paddle *left, paddle *right, unsigned short *fb, unsigned char *parlcd_mem_base, game *game);

#endif
