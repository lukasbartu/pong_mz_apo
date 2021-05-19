#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>


#include "game_fcn.h"

#include "game.h"
#include "utils.h"
#include "lcd_text.h"


#define DISPLAY_WIDTH 480
#define DISPLAY_HEIGHT 320

#define PADDLE_HEIGHT 70

void keyboard_game(unsigned char *mem_base, unsigned char *parlcd_mem_base, unsigned short *fb)
{
    bool quit=false;
    paddle left_paddle = initLeftpaddle();   //init
    paddle right_paddle = initRightpaddle(); //init
    ball ball = init_ball();

    init_ball();
    int ch = 0;
    game.score_p1=0;
    game.score_p2=0;

    left_paddle.position = 160;
    right_paddle.position = 160;

    while((game.score_p1 <= 5 && game.score_p2 <= 5) && !quit){

        left_paddle.speed = 0.0;
        right_paddle.speed = 0.0;
        if( kbhit() ){
            usleep(500);
            ch = getch();
            switch (ch) {
            case 'k':
                right_paddle.position += 5;
                left_paddle.speed = 0.5;
            break;
            case 'j':
                right_paddle.position -= 5;
                right_paddle.speed = 0.5;
            break;
            case 'd':
                left_paddle.position += 5;
                left_paddle.speed = 0.5;
            break;
            case 's':
                left_paddle.position -= 5;
                right_paddle.speed = 0.5;
            break;
            case 'q':
                quit = true;
            break;
            }
        }

        if (left_paddle.position <= 35){
            left_paddle.position = 35;
        }
        if (right_paddle.position <= 35){
            right_paddle.position = 35;
        }
        if (left_paddle.position >= 285){
            left_paddle.position = 285;
        }
        if (right_paddle.position >= 285){
            right_paddle.position = 285;
        }

        update_ball(mem_base, &ball, &left_paddle, &right_paddle, fb, parlcd_mem_base);
        
        left_paddle.offset = DISPLAY_WIDTH * (left_paddle.position - (PADDLE_HEIGHT / 2)); //set offset acording to position
    right_paddle.offset = DISPLAY_WIDTH * (right_paddle.position - (PADDLE_HEIGHT / 2));

    frame_buffer_clear(fb);

  for (int y = left_paddle.offset + left_paddle.start;
       y < left_paddle.offset + left_paddle.start + (PADDLE_HEIGHT * DISPLAY_WIDTH);
       y += DISPLAY_WIDTH)
  { //increment 90 lines of height
    for (int x = 0; x < left_paddle.thicness; x++)
    {                     //increment 30px width - starts at left offset
      fb[y + x] = 0xFC1F; //make box of pink (I like pink, white will be eventually)
      //printf("FB y- %d; x- %d \n", y, x); //just to see what is the hell going on
    }
  }

  for (int y = right_paddle.offset + right_paddle.start;
       y < right_paddle.offset + right_paddle.start + (PADDLE_HEIGHT * DISPLAY_WIDTH);
       y += DISPLAY_WIDTH)
  { //increment 90 lines of height
    for (int x = 0; x < right_paddle.thicness; x++)
    {                     //increment 30px width - starts at left offset
      fb[y + x] = 0x801F; //make box of purple (at the end we will have rainbow)
      //printf("FB y- %d; x- %d \n", y, x);
    }
  }

  //ball to fb
  ball.offset = DISPLAY_WIDTH * (ball.pos_y - (18 / 2)); // 18x18px ball 

    for (int y = ball.offset + (ball.pos_x-(18/2));
        y < ball.offset + (ball.pos_x-(18/2)) + (18 * DISPLAY_WIDTH);
        y += DISPLAY_WIDTH)
    { //increment 90 lines of height
      for (int x = 0; x < 18; x++)
      {                     //increment 30px width - starts at left offset
        fb[y + x] = 0x801F; //make box of purple (at the end we will have rainbow)
        //printf("FB y- %d; x- %d \n", y, x);
      }
    }

    lcd_draw(parlcd_mem_base, fb);

    }
}

void knob_game(unsigned char *mem_base, unsigned char *parlcd_mem_base, unsigned short *fb)
{
    paddle left_paddle = initLeftpaddle();   //init
    paddle right_paddle = initRightpaddle(); //init
    ball ball = init_ball();

    init_ball();
    game.score_p1=0;
    game.score_p2=0;

    while(game.score_p1 <= 5 && game.score_p2 <= 5){

        update_paddle_position(mem_base, &left_paddle, &right_paddle);
        update_ball(mem_base, &ball, &left_paddle, &right_paddle, fb, parlcd_mem_base);
        
        left_paddle.offset = DISPLAY_WIDTH * (left_paddle.position - (PADDLE_HEIGHT / 2)); //set offset acording to position
    right_paddle.offset = DISPLAY_WIDTH * (right_paddle.position - (PADDLE_HEIGHT / 2));

    frame_buffer_clear(fb);

  for (int y = left_paddle.offset + left_paddle.start;
       y < left_paddle.offset + left_paddle.start + (PADDLE_HEIGHT * DISPLAY_WIDTH);
       y += DISPLAY_WIDTH)
  { //increment 90 lines of height
    for (int x = 0; x < left_paddle.thicness; x++)
    {                     //increment 30px width - starts at left offset
      fb[y + x] = 0xFC1F; //make box of pink (I like pink, white will be eventually)
      //printf("FB y- %d; x- %d \n", y, x); //just to see what is the hell going on
    }
  }

  for (int y = right_paddle.offset + right_paddle.start;
       y < right_paddle.offset + right_paddle.start + (PADDLE_HEIGHT * DISPLAY_WIDTH);
       y += DISPLAY_WIDTH)
  { //increment 90 lines of height
    for (int x = 0; x < right_paddle.thicness; x++)
    {                     //increment 30px width - starts at left offset
      fb[y + x] = 0x801F; //make box of purple (at the end we will have rainbow)
      //printf("FB y- %d; x- %d \n", y, x);
    }
  }

  //ball to fb
  ball.offset = DISPLAY_WIDTH * (ball.pos_y - (18 / 2)); // 18x18px ball 

    for (int y = ball.offset + (ball.pos_x-(18/2));
        y < ball.offset + (ball.pos_x-(18/2)) + (18 * DISPLAY_WIDTH);
        y += DISPLAY_WIDTH)
    { //increment 90 lines of height
      for (int x = 0; x < 18; x++)
      {                     //increment 30px width - starts at left offset
        fb[y + x] = 0x801F; //make box of purple (at the end we will have rainbow)
        //printf("FB y- %d; x- %d \n", y, x);
      }
    }

    lcd_draw(parlcd_mem_base, fb);

    }
}
