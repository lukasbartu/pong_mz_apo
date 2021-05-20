#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "game_fnc.h"
#include "game.h"
#include "lcd_text.h"
#include "colors.h"
#include "font_types.h"
#include "utils.h"

#define DISPLAY_WIDTH 480
#define DISPLAY_HEIGHT 320

int main(int argc, char *argv[])
{
    unsigned short *fb;
    unsigned char *mem_base;
    unsigned char *parlcd_mem_base;
    uint32_t val_line = 5;
    printf("Alloc FB\n");
    fb = (unsigned short *)malloc(DISPLAY_HEIGHT * DISPLAY_WIDTH * 2); //allocates frame buffer
    if (!fb){
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
    for (int i = 0; i < 30; i++){
        *(volatile uint32_t *)(mem_base + SPILED_REG_LED_LINE_o) = val_line;
        val_line <<= 1;
        printf("LED val 0x%x\n", val_line);
        clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
    }

    parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
    /* If mapping fails exit with error code */
    if (parlcd_mem_base == NULL){
        exit(1);
    }

    printf("Font descriptors\n");
    font_descriptor_t *fdes88 = &font_wArial_88; //  font template
    font_descriptor_t *fdes44 = &font_wArial_44; //  font template

    frame_buffer_clear(fb);

    // start of start-up screen ----------------------------------------------------
    draw_string(105, 60, "PONG", 4, WHITE_COLOR, fdes88, fb);
    draw_string(80, 250, "PRESS ENTER!", 11, WHITE_COLOR, fdes44, fb);
    lcd_draw(parlcd_mem_base, fb);

    printf("PRESS ENTER TO CONTINUE!\n");
    getchar(); // wait for enter
    // end of start-up screen --------------------------------------------------------

    frame_buffer_clear(fb);

    // start of menu screen ----------------------------------------------------------
    draw_string(105, 15, "MENU", 4, YELLOW_COLOR, fdes88, fb);
    draw_string(30, 100, "Keyboard mode", 13, BLUE_COLOR, fdes44, fb); // option 0
    draw_string(10, 140, "Knob mode", 10, YELLOW_COLOR, fdes44, fb);   // option 1
    draw_string(10, 180, "exit", 11, YELLOW_COLOR, fdes44, fb);        // option 2
    lcd_draw(parlcd_mem_base, fb);

    char c = '0';
    int option = 0;
    bool quit = false;
    bool newentry = false;
    uint32_t rgb_knobs_value;
    uint8_t pushedknob;
    while (!quit){
        if (kbhit()){
            c = getch();
        }
        rgb_knobs_value = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
        pushedknob = (uint8_t)(rgb_knobs_value >> 24);

        if (pushedknob == 0x04 || c == '-'){ //red knob || -
            if (option - 1 < 0){
                option = 2;
            }else{
                option = option - 1;
            }
            c = '0';
            newentry = true;
        }else if (pushedknob == 0x01 || c == '+'){ //blue knob || +
            if (option + 1 > 2){
                option = 0;
            }else{
                option = option + 1;
            }
            c = '0';
            newentry = true;
        }else if (pushedknob == 0x02 || c == 0x0A){ //green knob || enter
            c = '0';
            newentry = true;
            if (option == 0){
                keyboard_game(mem_base, parlcd_mem_base, fb);
            }else if (option == 1){
                knob_game(mem_base, parlcd_mem_base, fb);
            }else if (option == 2){
                quit = true;
            }
        }

        if (newentry){
            newentry = false;
            switch (option)
            {
            case 0: // option 0 selected
                frame_buffer_clear(fb);
                draw_string(105, 15, "MENU", 4, YELLOW_COLOR, fdes88, fb);
                draw_string(30, 100, "Keyboard mode", 13, BLUE_COLOR, fdes44, fb); // option 0
                draw_string(10, 140, "Knob mode", 10, YELLOW_COLOR, fdes44, fb);   // option 1
                draw_string(10, 180, "exit", 11, YELLOW_COLOR, fdes44, fb);        // option 2
                lcd_draw(parlcd_mem_base, fb);
                break;
            case 1: // option 1 selected
                frame_buffer_clear(fb);
                draw_string(105, 15, "MENU", 4, YELLOW_COLOR, fdes88, fb);
                draw_string(10, 100, "Keyboard mode", 13, YELLOW_COLOR, fdes44, fb); // option 0
                draw_string(30, 140, "Knob mode", 10, BLUE_COLOR, fdes44, fb);       // option 1
                draw_string(10, 180, "exit", 11, YELLOW_COLOR, fdes44, fb);          // option 2
                lcd_draw(parlcd_mem_base, fb);
                break;
            case 2: // option 2 selected
                frame_buffer_clear(fb);
                draw_string(105, 15, "MENU", 4, YELLOW_COLOR, fdes88, fb);
                draw_string(10, 100, "Keyboard mode", 13, YELLOW_COLOR, fdes44, fb); // option 0
                draw_string(10, 140, "Knob mode", 10, YELLOW_COLOR, fdes44, fb);     // option 1
                draw_string(30, 180, "exit", 11, BLUE_COLOR, fdes44, fb);            // option 2
                lcd_draw(parlcd_mem_base, fb);
                break;
            default:
                break;
            }
        }
    }

    frame_buffer_clear(fb);
    draw_string(130, 160, "END", 3, WHITE_COLOR, fdes88, fb);
    lcd_draw(parlcd_mem_base, fb);
    // end of menu screen ------------------------------------------------------------

    if (fb){
        free(fb);
    }
    printf("Goodbye world\n");

    return 0;
}
