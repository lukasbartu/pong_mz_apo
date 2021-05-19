#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "game_fcn.h"
#include "game.h"
#include "lcd_text.h"
#include "colors.h"
#include "font_types.h"
#include "utils.h"


#define DISPLAY_WIDTH 480
#define DISPLAY_HEIGHT 320

int main(int argc, char *argv[])
{
    unsigned char *mem_base;
    unsigned char *parlcd_mem_base;
    uint32_t val_line = 5;
    printf("Alloc FB\n");
    game.fb = (unsigned short *)malloc(DISPLAY_HEIGHT * DISPLAY_WIDTH * 2); //allocates frame buffer
    if(!game.fb){
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
    for (int i = 0; i < 30; i++)
    {
        *(volatile uint32_t *)(mem_base + SPILED_REG_LED_LINE_o) = val_line;
        val_line <<= 1;
        printf("LED val 0x%x\n", val_line);
        clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
    }

    parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
  /* If mapping fails exit with error code */
    if (parlcd_mem_base == NULL)
        exit(1);

    printf("Font descriptors\n");
    font_descriptor_t* fdes88 = &font_wArial_88; //  font template
    font_descriptor_t* fdes44 = &font_wArial_44; //  font template
    
    frame_buffer_clear(game.fb);

    // start of start-up screen ----------------------------------------------------
    draw_string(105,60,"PONG",4,WHITE_COLOR,fdes88,game.fb);
    draw_string(80,250,"PRESS ENTER!",11,WHITE_COLOR,fdes44,game.fb); 
    lcd_draw(parlcd_mem_base, game.fb);

    printf("PRESS ENTER TO CONTINUE!\n");
    getchar(); // wait for enter
    // end of start-up screen --------------------------------------------------------

    frame_buffer_clear(game.fb);

    // start of menu screen ----------------------------------------------------------
    draw_string(105,15,"MENU",4,YELLOW_COLOR,fdes88,game.fb);
    draw_string(30,100,"Keyboard mode",13,BLUE_COLOR,fdes44,game.fb); // option 0
    draw_string(10,140,"Knob mode",10,YELLOW_COLOR,fdes44,game.fb);  // option 1
    draw_string(10,180,"Arcade mode",11,YELLOW_COLOR,fdes44,game.fb); // option 2
    lcd_draw(parlcd_mem_base, game.fb);


    char c;
    int option = 0;
    while((c = getch()) != 'q'){
        switch (c)
        {
        case '-': // menu up
            if(option-1 < 0 ){
                option = 2;
            }else{
                option = option -1; 
            }
            break;
        case '+': // menu down
            if(option+1 > 2 ){
                option = 0;
            }else{
                option = option +1; 
            }
            break;
        case 0x0A: // enter
            if(option == 0){
                keyboard_game(mem_base, parlcd_mem_base, game.fb);
            }else if(option == 1){
                knob_game(mem_base, parlcd_mem_base, game.fb);
            }
            break;
        default:
            break;
        }
        switch (option)
        {
        case 0: // option 0 selected
            frame_buffer_clear(game.fb);
            draw_string(105,15,"MENU",4,YELLOW_COLOR,fdes88,game.fb);
            draw_string(30,100,"Keyboard mode",13,BLUE_COLOR,fdes44,game.fb); // option 0
            draw_string(10,140,"Knob mode",10,YELLOW_COLOR,fdes44,game.fb);  // option 1
            draw_string(10,180,"Arcade mode",11,YELLOW_COLOR,fdes44,game.fb); // option 2
            lcd_draw(parlcd_mem_base, game.fb);
            break;
        case 1: // option 1 selected
            frame_buffer_clear(game.fb);
            draw_string(105,15,"MENU",4,YELLOW_COLOR,fdes88,game.fb);
            draw_string(10,100,"Keyboard mode",13,YELLOW_COLOR,fdes44,game.fb); // option 0
            draw_string(30,140,"Knob mode",10,BLUE_COLOR,fdes44,game.fb);  // option 1   
            draw_string(10,180,"Arcade mode",11,YELLOW_COLOR,fdes44,game.fb); // option 2 
            lcd_draw(parlcd_mem_base, game.fb); 
            break;
        case 2: // option 2 selected
            frame_buffer_clear(game.fb);
            draw_string(105,15,"MENU",4,YELLOW_COLOR,fdes88,game.fb);
            draw_string(10,100,"Keyboard mode",13,YELLOW_COLOR,fdes44,game.fb); // option 0
            draw_string(10,140,"Knob mode",10,YELLOW_COLOR,fdes44,game.fb);  // option 1
            draw_string(30,180,"Arcade mode",11,BLUE_COLOR,fdes44,game.fb); // option 2
            lcd_draw(parlcd_mem_base, game.fb);
            break;
        default:
            break;
        }  
    }

    frame_buffer_clear(game.fb);
    draw_string(130,160,"END",3,WHITE_COLOR,fdes88,game.fb);
    lcd_draw(parlcd_mem_base, game.fb);
// end of menu screen ------------------------------------------------------------

    if(game.fb){
        free(game.fb);
    }
    printf("Goodbye world\n");

  return 0;
}
