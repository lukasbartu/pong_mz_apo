/*******************************************************************
  Edited template for MZ_APO - paddles acording to knobs - starting 
  to work with FB and knobs

 *******************************************************************/

#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "game_fcn.h"
#include "lcd_text.h"

#include "font_types.h"

#define DISPLAY_WIDTH 480
#define DISPLAY_HEIGHT 320


#define PADDLE_THICNESS 30 //px

void lcd_draw(unsigned char *parlcd_mem_base)
{
  printf("Draw LCD\n"); //now draw the FB on the screen
  parlcd_write_cmd(parlcd_mem_base, 0x2c);
  for (int ptr = 0; ptr < DISPLAY_WIDTH * DISPLAY_HEIGHT; ptr++)
  {
    parlcd_write_data(parlcd_mem_base, game->fb[ptr]);
  }
  printf("drawn LCD\n");
}


#define PADDLE_HEIGHT 90   //px
#define KNOB_MAX_VALUE 0xFF //used to calculate relative position of knob
int main(int argc, char *argv[])
{
  unsigned char *mem_base;
  unsigned char *parlcd_mem_base;
  uint32_t val_line = 5;
  int i, j;
  int ptr;
  printf("alloc FB\n");
  game->fb = (unsigned short *)malloc(DISPLAY_HEIGHT * DISPLAY_WIDTH * 2); //allocates frame buffer
  if(!game->fb){
    exit(1);
  }
  printf("Hello world\n");

  sleep(1);

  /*
   * Setup memory mapping which provides access to the peripheral
   * registers region of RGB LEDs, knobs and line of yellow LEDs.
   */
  mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
  /* If mapping fails exit with error code */
  if (mem_base == NULL)
    exit(1);

  struct timespec loop_delay = {.tv_sec = 0, .tv_nsec = 20 * 1000 * 1000};
  for (i = 0; i < 30; i++)
  {
    *(volatile uint32_t *)(mem_base + SPILED_REG_LED_LINE_o) = val_line;
    val_line <<= 1;
    printf("LED val 0x%x\n", val_line);
    clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
  }

  parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);

  if (parlcd_mem_base == NULL)
    exit(1);

  parlcd_hx8357_init(parlcd_mem_base);

  parlcd_write_cmd(parlcd_mem_base, 0x2c); //makes clear screen - well, not clear, but inicialize F(rame)B(uffer) to black
  ptr = 0;
  for (i = 0; i < DISPLAY_HEIGHT; i++)
  {
    for (j = 0; j < DISPLAY_WIDTH; j++)
    {
      game->fb[ptr] = 0x0000; //0x0000 = black  /*for colours in hex from rgb - http://www.barth-dev.de/online/rgb565-color-picker/ */
      parlcd_write_data(parlcd_mem_base, fb[ptr++]);
    }
  }

  /*
      * Access register holding 8 bit relative knobs position
      * The type "(volatile uint32_t*)" casts address obtained
      * as a sum of base address and register offset to the
      * pointer type which target in memory type is 32-bit unsigned
      * integer. The "volatile" keyword ensures that compiler
      * cannot reuse previously read value of the location.
      */

  uint32_t rgb_knobs_value;
  rgb_knobs_value = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
  printf("rgb_knobs_value %x\n", rgb_knobs_value);  


  /* Store the read value to the register controlling individual LEDs */
  *(volatile uint32_t *)(mem_base + SPILED_REG_LED_LINE_o) = rgb_knobs_value;
  uint32_t led_rgb = *(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB1_o);
  printf("rgb val %x\n", led_rgb);  

  *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB2_o) = 0x00B5CEB1;

  /* use this way to split data in register to bytes 
    uint32 tmp;
    uint8 b3 = (uint8)(tmp>>24); 
    uint8 b2 = (uint8)(tmp>>16); 
    uint8 b1 = (uint8)(tmp>>8); 
    uint8 b0 = (uint8)(tmp>>0);*/

  //splits knobs value to two variables

  paddle left_paddle = initLeftpaddle();   //init
  paddle right_paddle = initRightpaddle(); //init

  update_paddle_position(mem_base, &left_paddle.position , &right_paddle.position);

  
  left_paddle.offset = DISPLAY_WIDTH * (left_paddle.position - (PADDLE_HEIGHT / 2)); //set offset acording to position
  right_paddle.offset = DISPLAY_WIDTH * (right_paddle.position - (PADDLE_HEIGHT / 2));

  for (int y = left_paddle.offset + left_paddle.start;
       y < left_paddle.offset + left_paddle.start + (PADDLE_HEIGHT * DISPLAY_WIDTH);
       y += DISPLAY_WIDTH)
  { //increment 90 lines of height
    for (int x = 0; x < left_paddle.thicness; x++)
    {                     //increment 30px width - starts at left offset
      game->fb[y + x] = 0xFC1F; //make box of pink (I like pink, white will be eventually)
      //printf("FB y- %d; x- %d \n", y, x); //just to see what is the hell going on
    }
  }

  for (int y = right_paddle.offset + right_paddle.start;
       y < right_paddle.offset + right_paddle.start + (PADDLE_HEIGHT * DISPLAY_WIDTH);
       y += DISPLAY_WIDTH)
  { //increment 90 lines of height
    for (int x = 0; x < right_paddle.thicness; x++)
    {                     //increment 30px width - starts at left offset
      game->fb[y + x] = 0x801F; //make box of purple (at the end we will have rainbow)
      //printf("FB y- %d; x- %d \n", y, x);
    }
  }

  lcd_draw(parlcd_mem_base);

  /*end of drawing paddles*/

  //  font template
  int x = 95;
  char str[]="INSERT";
  char *ch=str;
  char str2[]="coin";
  printf("font des\n");
  font_descriptor_t* fdes = &font_wArial_88;
  
  for (ptr = 0; ptr < 320*480 ; ptr++) {
    fb[ptr]=0x0000; //0u - unsigned int
  }
  for (i=0; i<6; i++) {
    printf("Draw char %c\n", *ch);
    draw_char(x, 60, fdes, *ch, game->fb);
    x+=char_width(fdes, *ch);
    ch++;
  }
  x = 167;
  ch=str2;
  for (i=0; i<4; i++) {
    printf("Draw char %c\n", *ch);
    draw_char(x, 155, fdes, *ch, game->fb);
    x+=char_width(fdes, *ch);
    ch++;
  }
  
  lcd_draw(parlcd_mem_base);
  
  printf("Goodbye world\n");

  return 0;
}
