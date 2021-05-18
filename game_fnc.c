#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>


#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "font_types.h"
#include "lcd_text.h"

#include "game_fcn.h"


#define PADDLE_THICNESS 30 //px
#define PADDLE_HEIGHT 90   //px
#define KNOB_MAX_VALUE 0xFF //used to calculate relative position of knob

/*inicialize paddles to start positions*/
paddle initLeftpaddle()
{
  paddle left_paddle;
  left_paddle.start = 10;
  left_paddle.thicness = PADDLE_THICNESS; //basicly constant
  left_paddle.edge = left_paddle.start + PADDLE_THICNESS;
  return left_paddle;
}
paddle initRightpaddle()
{
  paddle right_paddle;
  right_paddle.start = 440;
  right_paddle.thicness = PADDLE_THICNESS;
  right_paddle.edge = right_paddle.start;
  return right_paddle;
}


void update_paddle_position(unsigned char *mem_base, int* pos_l , int* pos_r)
{
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
    *pos_l = (275 * percent_left) / 100;
    *pos_r = (275 * percent_right) / 100;
    

    if (*pos_l <= 45)
    {
      *pos_l = 45;
    }
    if (*pos_r <= 45)
    {
      *pos_r = 45;
    }
    if (*pos_l >= 275)
    {
      *pos_l = 275;
    }
    if (*pos_r >= 275)
    {
      *pos_r = 275;
    }
    printf("pos left %d\n", *pos_l);
    printf("pos right %d\n", *pos_r);

}   

void draw_score(void)
{
    font_descriptor_t* fdes = &font_wArial_88;
    char ch1 = game.score_p1 + '0'; //coverts score int to char for print
    char ch2 = '-';
    char ch3 = game.score_p2 + '0';

    draw_char(153 , 116, fdes, ch1, game.fb);
    draw_char(227, 116, fdes, ch2, game.fb);
    draw_char(283 , 116, fdes, ch3, game.fb);
}

void goal(int p, unsigned char *mem_base)
{
  if(p == 1){
    game.score_p1++;

    draw_score();

    for(int i=0; i<5; i++){
      *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB2_o) = 0x000086C0; //blue
      *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB1_o) = 0x000086C0; //blue
      sleep(0.2);
      *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB2_o) = 0x00000000; //off
      *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB1_o) = 0x00000000; //off
      sleep(0.2);
    }
  }else if(p == 2){
    game.score_p2++;

    draw_score();

    for(int i=0; i<5; i++){
      *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB2_o) = 0x00D32627; //red
      *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB1_o) = 0x00D32627; //red
      sleep(0.2);
      *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB2_o) = 0x00000000; //off
      *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB1_o) = 0x00000000; //off
      sleep(0.2);
    }
  }

}


#define DISPLAY_WIDTH 480
#define DISPLAY_HEIGHT 320
void update_ball(unsigned char *mem_base, ball *ball, paddle *left, paddle *right)
{
  game.goal = false;
  //check upper and lower bound
  if(ball->pos_y >= DISPLAY_HEIGHT || ball->pos_y <= 0){
    ball->dy = ball->dy *-1;
  }

  //check for left - right = goal
  if(ball->pos_x <= 0){
    goal(1, mem_base);
    game.goal = true;
  }else if(ball->pos_x >= DISPLAY_WIDTH){
    goal(2, mem_base);
    game.goal = true;
  }

  //collisions with paddles
  if(ball->pos_x <= left->edge 
  && ball->pos_y >= left->position - (PADDLE_HEIGHT/2) 
  && ball->pos_y <= left->position + (PADDLE_HEIGHT/2) ){
    ball->dx = ball->dx *-1;
  }
  if(ball->pos_x >= right->edge 
  && ball->pos_y >= left->position - (PADDLE_HEIGHT/2) 
  && ball->pos_y <= left->position + (PADDLE_HEIGHT/2) ){
    ball->dx = ball->dx *-1;
  }

  //move ball
  ball->pos_x += ball->dx;
  ball->pos_y += ball->dy;

}
