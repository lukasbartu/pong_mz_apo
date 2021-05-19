
#include "lcd_text.h"

#define DISPLAY_WIDTH 480
#define DISPLAY_HEIGHT 320

void draw_pixel(int x, int y, unsigned short color, unsigned short *fb)
{ //self explanatory, not yet used
  if (x >= 0 && x < DISPLAY_WIDTH && y >= 0 && y < DISPLAY_HEIGHT)
  {
    fb[x + DISPLAY_WIDTH * y] = color;
  }
}

void draw_char(int x, int y, font_descriptor_t *fdes, char ch, unsigned int color, unsigned short *fb)
{ 
  int nmbr = (int)ch - fdes->firstchar;//44;
  if(nmbr>fdes->size){
    if(fdes->defaultchar){
      nmbr = fdes->defaultchar;
    }else{
      nmbr = 0;
    }
  }
  printf("offset; char: %d\n", nmbr);
  uint32_t offset = *(fdes->offset+nmbr);
  printf("width\n");
  int width= *(fdes->width+nmbr);
  int tmp=0;
  int line=0;
  int x_tmp = x;
  printf("width while %d\n", width);
  while(width%16 != 0){
        width++;
  }
  printf("start drawing off: %d; width: %d\n", offset, width);
  for(int i=0; i<(width/16)*fdes->height; i++){
        //convert to bits
        uint16_t z = *((fdes->bits)+i+offset);
        for (int j = 0; j < 16; j++) {
            //printf("x %d, y %d\n", x+j, y+i);
            if(( (z & 0x8000) >> 15 ) == 1){
              //printf("x %d; y %d\n",x_tmp , y+line);
              draw_pixel(x_tmp, y+line, color, fb);
            }
            x_tmp++;
            z <<= 1;
        }
        if(tmp++>=(width/16)-1){
            tmp=0;
            line++;
            x_tmp = x;
        }
    }
  //printf("I have temp\n");

}

int char_width(font_descriptor_t *fdes, int ch)
{ //gets width of char
  int width = 0;
  if ((ch >= fdes->firstchar) && (ch - fdes->firstchar < fdes->size))
  {
    ch -= fdes->firstchar;
    if (!fdes->width)
    {
      width = fdes->maxwidth;
    }
    else
    {
      width = fdes->width[ch];
    }
  }
  return width;
}

void draw_string(int x, int y, char *string,  int length, unsigned int color, font_descriptor_t *fdes, unsigned short *fb){
  char *ch = string;
  for (int i=0; i<length; i++) {
    printf("Draw char %c\n", *ch);
    draw_char(x, y, fdes, *ch, color, fb);
    x+=char_width(fdes, *ch);
    ch++;
  }
}

void lcd_draw(unsigned char *parlcd_mem_base, unsigned short *fb)
{
  printf("Draw LCD\n");
  parlcd_write_cmd(parlcd_mem_base, 0x2c);
  for (int ptr = 0; ptr < DISPLAY_WIDTH * DISPLAY_HEIGHT; ptr++)
  {
    parlcd_write_data(parlcd_mem_base, fb[ptr]);
  }
  printf("Drawn LCD\n");
}


void frame_buffer_clear(unsigned short *fb){
  for (int ptr = 0; ptr < 320*480 ; ptr++) {
    fb[ptr]=0x0000; //0u - unsigned int
  }
}
