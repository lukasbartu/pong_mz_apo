#ifndef GAME_FCN_H
#define GAME_FCN_H

#include <stdbool.h>


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

static struct {
  unsigned short *fb;

  int score_p1;
  int score_p2;
  
  int goal;
} game = {
  .score_p1 = 0,
  .score_p2 = 0,

  .goal = 0
};

paddle initLeftpaddle();
paddle initRightpaddle();
ball init_ball();

void update_paddle_position(unsigned char *mem_base, paddle *left, paddle *right);

void draw_score(void);
void goal(int p, unsigned char *mem_base);

void update_ball(unsigned char *mem_base, ball *ball, paddle *left, paddle *right);

#endif
