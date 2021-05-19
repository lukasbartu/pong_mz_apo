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

#include "game_fcn.h"


#define PADDLE_THICNESS 20 //px
#define PADDLE_HEIGHT 70   //px
#define KNOB_MAX_VALUE 0xFF //used to calculate relative position of knob

/*inicialize paddles to start positions*/
paddle initLeftpaddle()
{
  paddle left_paddle;
  left_paddle.start = 10;
  left_paddle.thicness = PADDLE_THICNESS; //basicly constant
  left_paddle.edge = left_paddle.start + PADDLE_THICNESS;
  left_paddle.speed = 0;
  return left_paddle;
}
paddle initRightpaddle()
{
  paddle right_paddle;
  right_paddle.start = 440;
  right_paddle.thicness = PADDLE_THICNESS;
  right_paddle.edge = right_paddle.start;
  right_paddle.speed = 0;
  return right_paddle;
}

/*spawns ball in random 45° direction on center*/
ball init_ball()
{
  ball ball;
  ball.dx = 10 * 0.7 * rand_sign();
  ball.dy = 10 * 0.7 * rand_sign();
  
  ball.pos_y = 160;
  ball.pos_x = 240;

  return ball;
}

void update_paddle_position(unsigned char *mem_base, paddle *left, paddle *right)
{
    int oldpos_l = left->position;
    int oldpos_r = right->position;
    uint32_t rgb_knobs_value;
    rgb_knobs_value = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
    printf("rgb_knobs_value %x\n", rgb_knobs_value);  

    uint8_t blue_knob = (uint8_t)(rgb_knobs_value >> 0);
    uint8_t red_knob = (uint8_t)(rgb_knobs_value >> 16);

    printf("blue knob %x\n", blue_knob);
    printf("red knob %x\n", red_knob);

    uint16_t relative_left = red_knob * 0x64; //0x64 = 100decimal - rozsireni pro celociselne pocty procent
    uint16_t relative_right = blue_knob * 0x64;
    int percent_left = relative_left / KNOB_MAX_VALUE;
    int percent_right = relative_right / KNOB_MAX_VALUE;
    printf("percent left %d\n", percent_left);
    printf("percent right %d\n", percent_right);

    //asign position acordingly to knobs relative value
    left->position = (320 * percent_left) / 100;
    right->position = (320 * percent_right) / 100;
    

    if (left->position <= 35){
      left->position = 35;
    }
    if (right->position <= 35){
      right->position = 35;
    }
    if (left->position >= 285){
      left->position = 285;
    }
    if (right->position >= 285){
      right->position = 285;
    }
    left->position = 160;
    printf("pos left %d\n", left->position);
    printf("pos right %d\n", right->position);

  left->speed = (oldpos_l - left->position)/100;
  right->speed = (oldpos_r - right->position)/100;
}   


void draw_score()
{
    printf("font descripption\n");
    font_descriptor_t* fdes = &font_wArial_88;
    char ch1 = game.score_p1 + '0'; //coverts score int to char for print
    char ch2 = '-';
    char ch3 = game.score_p2 + '0';
    printf("ch 1: %c, ch 2: %c, ch 3: %c\n", ch1 ,ch2, ch3);

    draw_char(153 , 116, fdes, ch1, game.fb);
    draw_char(227, 116, fdes, ch2, game.fb);
    draw_char(283 , 116, fdes, ch3, game.fb);
}

void goal(int p, unsigned char *mem_base)
{
  if(p == 1){
    //draw_score();
    
    for(int i=0; i<5; i++){
      *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB2_o) = 0x000086C0; //blue
      *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB1_o) = 0x000086C0; //blue
      usleep(300000);
      *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB2_o) = 0x00000000; //off
      *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB1_o) = 0x00000000; //off
      usleep(300000);
    }
  }else if(p == 2){
    //draw_score();

    for(int i=0; i<5; i++){
      *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB2_o) = 0x00D32627; //red
      *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB1_o) = 0x00D32627; //red
      usleep(300000);
      *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB2_o) = 0x00000000; //off
      *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB1_o) = 0x00000000; //off
      usleep(300000);
    }
  }

}


#define DISPLAY_WIDTH 480
#define DISPLAY_HEIGHT 320
void update_ball(unsigned char *mem_base, ball *ball, paddle *left, paddle *right)
{
  game.goal = 0;
  printf("check upper and lower bound\n");
  //check upper and lower bound
  if(ball->pos_y+19 >= DISPLAY_HEIGHT || ball->pos_y-19 <= 0){
    ball->dy = ball->dy *-1.0;
  }

  printf("check for left - right\n");
  //check for left - right = goal
  if(ball->pos_x-19 <= 0){
    game.score_p2 += 1;
    printf("goal\n");
    goal(1, mem_base);
    game.goal = 1;
  }
  if(ball->pos_x+19 >= DISPLAY_WIDTH){
    game.score_p1 += 1;
    printf("goal\n");
    goal(2, mem_base);
    game.goal = 2;
  }

  printf("collisions with paddles\n");
  //collisions with paddles
  if(ball->pos_x-1 <= left->edge 
  && ball->pos_y >= left->position - (PADDLE_HEIGHT/2) 
  && ball->pos_y <= left->position + (PADDLE_HEIGHT/2) ){
    //ball->dx = ball->dx *-1;
    ball->dy = ball->dy + (10 * left->speed);
    ball->dx = sqrt( ((100)-(pow(ball->dy,2))) );
    printf("collied\n");
  }
  if(ball->pos_x+19 >= right->edge 
  && ball->pos_y >= right->position - (PADDLE_HEIGHT/2) 
  && ball->pos_y <= right->position + (PADDLE_HEIGHT/2) ){
    //ball->dx = ball->dx *-1;
    ball->dy = ball->dy + (10 * right->speed);
    ball->dx = sqrt( ((100)-(pow(ball->dy,2))) ) * -1;
    printf("collied\n");
  }

  //move ball
  if(game.goal){  //sets ball to one of the players with random direction
    ball->dy = 10 * 0.7 * rand_sign();
    if(game.goal==1){
      ball->pos_y = left->position;
      ball->pos_x = 70;
      ball->dx = 10 * 0.7;
    }else if(game.goal==2){
      ball->pos_y = right->position;
      ball->pos_x = 410;
      ball->dx = -10 * 0.7;
    }
  }else{  //no goal, just move ball
    ball->pos_x += ball->dx;
    ball->pos_y += ball->dy;
  }

  if (ball->pos_x <= 0){
      ball->pos_x = 0;
    }
    if (ball->pos_y <= 18){
      ball->pos_y = 18;
    }
    if (ball->pos_x >= 462){
      ball->pos_x = 462;
    }
    if (ball->pos_y >= 302){
      ball->pos_y = 302;
    }
    printf("pos ball x%d ,y%d; smer: dx%f, dy%f\n", ball->pos_x, ball->pos_y, ball->dx, ball->dy);

}
