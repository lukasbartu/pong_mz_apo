/*******************************************************************
  Edited template for MZ_APO - hardcoded paddles - starting to work
  with FB

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

#include "font_types.h"

unsigned short *fb;

void draw_pixel(int x, int y, unsigned short color) {
  if (x>=0 && x<480 && y>=0 && y<320) {
    fb[x+480*y] = color;
  }
}

void draw_char(int x, int y, font_descriptor_t* fdes, char ch) {
}

int char_width(font_descriptor_t* fdes, int ch) {
  int width = 0;
  if ((ch >= fdes->firstchar) && (ch-fdes->firstchar < fdes->size)) {
    ch -= fdes->firstchar;
    if (!fdes->width) {
      width = fdes->maxwidth;
    } else {
      width = fdes->width[ch];
    }
  }
  return width;
}

int main(int argc, char *argv[]) {
  unsigned char *mem_base;
  unsigned char *parlcd_mem_base;
  uint32_t val_line=5;
  int i,j;
  int ptr;
  fb  = (unsigned short *)malloc(320*480*2);  //allocates frame buffer
  
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
  for (i=0; i<30; i++) {
     *(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = val_line;
     val_line<<=1;
     printf("LED val 0x%x\n", val_line);
     clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
  }
  
  parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);

  if (parlcd_mem_base == NULL)
    exit(1);

  parlcd_hx8357_init(parlcd_mem_base);

  parlcd_write_cmd(parlcd_mem_base, 0x2c);  //makes clear screen - well, not clear, but inicialize F(rame)B(uffer) to black
  ptr=0;
  for (i = 0; i < 320 ; i++) {
    for (j = 0; j < 480 ; j++) {
      fb[ptr]=0x0000;
      parlcd_write_data(parlcd_mem_base, fb[ptr++]);
    }
  }

  int start1 = 480*100;  //width * offset from top
  for(int y=start1; y<start1+(90*480);){  //increment 90 lines of height
    for(int x=10; x<10+30; x++){  //increment 30px width - starts at left offset
        fb[y+x] = 0xFC1F; //make box of pink (I like pink, white will be eventually)
        printf("FB y- %d; x- %d \n", y, x); //just to see what is the hell going on
    }
     y+=480;
  }

  int start2 = 480*100; //width * offset from top
  for(int y=start2; y<start2+(90*480);){  //increment 90 lines of height
    for(int x=440; x<440+30; x++){  //increment 30px width - starts at left offset
        fb[y+x] = 0x801F; //make box of purple (at the end we will have rainbow)
        printf("FB y- %d; x- %d \n", y, x);
    }
     y+=480;
  }
  
  printf("Draw LCD\n"); //now draw the FB on the screen
  parlcd_write_cmd(parlcd_mem_base, 0x2c);
    for (ptr = 0; ptr < 480*320 ; ptr++) {
        parlcd_write_data(parlcd_mem_base, fb[ptr]);
    }
  printf("drawn LCD\n");

  /*  font template - yet to be understood
  int x = 10;
  char str[]="Goodbye world";
  char *ch=str;
  font_descriptor_t* fdes = &font_winFreeSystem14x16;
  
  for (ptr = 0; ptr < 320*480 ; ptr++) {
    fb[ptr]=0u;
  }
  for (i=0; i<13; i++) {
    draw_char(x, 10, fdes, *ch);
    x+=char_width(fdes, *ch);
    ch++;
  }*/
    
  printf("Goodbye world\n");

  return 0;
}