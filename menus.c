
#include "menus.h"

settings_struct settings_menu(unsigned char *mem_base,unsigned char *parlcd_mem_base, unsigned short *fb){
    settings_struct settings = {
    .ball_speed = SPEED_1,
    .paddle_speed = SPEED_1,
    .player_one_color = COLOR_1, 
    .player_two_color = COLOR_1,
    };

    font_descriptor_t *fdes88 = &font_wArial_88; //  font template
    font_descriptor_t *fdes44 = &font_wArial_44; //  font template

    char c = '0';
    int option = 0;
    bool quit = false;
    bool newentry = true;
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
            if (option + 1 > 4){
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
                settings.ball_speed = cycle_speed(settings.ball_speed);
            }else if (option == 1){
                settings.paddle_speed = cycle_speed(settings.paddle_speed);
            }else if (option == 2){
                settings.player_one_color = cycle_colors(settings.player_one_color);
            }else if (option == 3){
                settings.player_two_color = cycle_colors(settings.player_two_color);
            }else if (option == 4){
                quit = true;
            }
        }

        if (newentry){  
            newentry = false;
            switch (option)
            {
            case 0: // option 0 selected
                frame_buffer_clear(fb);
                draw_string(35, 15, "SETTINGS", 9, YELLOW_COLOR, fdes88, fb);
                draw_string(30, 100, "Ball speed", 10, BLUE_COLOR, fdes44, fb);                     // option 0
                draw_string(300, 100, ((settings.ball_speed == 1) ? "1X" : (settings.ball_speed == 2) ? "2X" : "0.5X"), (settings.ball_speed == 0.5) ? 4 : 2, WHITE_COLOR, fdes44, fb );
                draw_string(10, 140, "Paddle speed", 12, YELLOW_COLOR, fdes44, fb);                 // option 1
                draw_string(300, 140, ((settings.paddle_speed == 1) ? "1X" : (settings.paddle_speed == 2) ? "2X" : "0.5X"), (settings.paddle_speed == 0.5) ? 4 : 2, WHITE_COLOR, fdes44, fb );
                draw_string(10, 180, "Player 1 colour", 15, YELLOW_COLOR, fdes44, fb);               // option 2
                draw_string(10, 220, "Player 2 colour", 15, YELLOW_COLOR, fdes44, fb);               // option 3
                draw_string(10, 260, "Back", 4, YELLOW_COLOR, fdes44, fb);                          // option 4
                lcd_draw(parlcd_mem_base, fb);
                break;
            case 1: // option 1 selected
                frame_buffer_clear(fb);
                draw_string(35, 15, "SETTINGS", 9, YELLOW_COLOR, fdes88, fb);
                draw_string(10, 100, "Ball speed", 10, YELLOW_COLOR, fdes44, fb);                   // option 0
                draw_string(300, 100, ((settings.ball_speed == 1) ? "1X" : (settings.ball_speed == 2) ? "2X" : "0.5X"), (settings.ball_speed == 0.5) ? 4 : 2, WHITE_COLOR, fdes44, fb );
                draw_string(30, 140, "Paddle speed", 12, BLUE_COLOR, fdes44, fb);                 // option 1
                draw_string(300, 140, ((settings.paddle_speed == 1) ? "1X" : (settings.paddle_speed == 2) ? "2X" : "0.5X"), (settings.paddle_speed == 0.5) ? 4 : 2, WHITE_COLOR, fdes44, fb );
                draw_string(10, 180, "Player 1 colour", 15, YELLOW_COLOR, fdes44, fb);               // option 2
                draw_string(10, 220, "Player 2 colour", 15, YELLOW_COLOR, fdes44, fb);               // option 3
                draw_string(10, 260, "Back", 4, YELLOW_COLOR, fdes44, fb);                          // option 4
                lcd_draw(parlcd_mem_base, fb);
                break;
            case 2: // option 2 selected
                frame_buffer_clear(fb);
                draw_string(35, 15, "SETTINGS", 9, YELLOW_COLOR, fdes88, fb);
                draw_string(10, 100, "Ball speed", 10, YELLOW_COLOR, fdes44, fb);                   // option 0
                draw_string(300, 100, ((settings.ball_speed == 1) ? "1X" : (settings.ball_speed == 2) ? "2X" : "0.5X"), (settings.ball_speed == 0.5) ? 4 : 2, WHITE_COLOR, fdes44, fb );
                draw_string(10, 140, "Paddle speed", 12, YELLOW_COLOR, fdes44, fb);                 // option 1
                draw_string(300, 140, ((settings.paddle_speed == 1) ? "1X" : (settings.paddle_speed == 2) ? "2X" : "0.5X"), (settings.paddle_speed == 0.5) ? 4 : 2, WHITE_COLOR, fdes44, fb );
                draw_string(30, 180, "Player 1 colour", 15, settings.player_one_color, fdes44, fb);  // option 2
                draw_string(10, 220, "Player 2 colour", 15, YELLOW_COLOR, fdes44, fb);               // option 3
                draw_string(10, 260, "Back", 4, YELLOW_COLOR, fdes44, fb);                          // option 4
                lcd_draw(parlcd_mem_base, fb);
                break;
            case 3: // option 3 selected
                frame_buffer_clear(fb);
                draw_string(35, 15, "SETTINGS", 9, YELLOW_COLOR, fdes88, fb);
                draw_string(10, 100, "Ball speed", 10, YELLOW_COLOR, fdes44, fb);                   // option 0
               draw_string(300, 100, ((settings.ball_speed == 1) ? "1X" : (settings.ball_speed == 2) ? "2X" : "0.5X"), (settings.ball_speed == 0.5) ? 4 : 2, WHITE_COLOR, fdes44, fb );
                draw_string(10, 140, "Paddle speed", 12, YELLOW_COLOR, fdes44, fb);                 // option 1
                draw_string(300, 140, ((settings.paddle_speed == 1) ? "1X" : (settings.paddle_speed == 2) ? "2X" : "0.5X"), (settings.paddle_speed == 0.5) ? 4 : 2, WHITE_COLOR, fdes44, fb );
                draw_string(10, 180, "Player 1 colour", 15, YELLOW_COLOR, fdes44, fb);               // option 2
                draw_string(30, 220, "Player 2 colour", 15, settings.player_two_color, fdes44, fb);  // option 3
                draw_string(10, 260, "Back", 4, YELLOW_COLOR, fdes44, fb);                          // option 4
                lcd_draw(parlcd_mem_base, fb);
                break;
            case 4: // option 4 selected
                frame_buffer_clear(fb);
                draw_string(35, 15, "SETTINGS", 9, YELLOW_COLOR, fdes88, fb);
                draw_string(10, 100, "Ball speed", 10, YELLOW_COLOR, fdes44, fb);                    // option 0
                draw_string(300, 100, ((settings.ball_speed == 1) ? "1X" : (settings.ball_speed == 2) ? "2X" : "0.5X"), (settings.ball_speed == 0.5) ? 4 : 2, WHITE_COLOR, fdes44, fb );
                draw_string(10, 140, "Paddle speed", 12, YELLOW_COLOR, fdes44, fb);                 // option 1
                draw_string(300, 140, ((settings.paddle_speed == 1) ? "1X" : (settings.paddle_speed == 2) ? "2X" : "0.5X"), (settings.paddle_speed == 0.5) ? 4 : 2, WHITE_COLOR, fdes44, fb );
                draw_string(10, 180, "Player 1 colour", 15, YELLOW_COLOR, fdes44, fb);                // option 2
                draw_string(10, 220, "Player 2 colour", 15, YELLOW_COLOR, fdes44, fb);                // option 3
                draw_string(30, 260, "Back", 4, BLUE_COLOR, fdes44, fb);                             // option 4
                lcd_draw(parlcd_mem_base, fb);
                break;
            default:
                break;
            }
        }
    }
    return settings;
}

float cycle_speed(float speed){
    if((speed == SPEED_1)){
        speed = SPEED_2;
    }else if((speed == SPEED_2)){
        speed = SPEED_3;
    }else if((speed = SPEED_3)){
        speed = SPEED_1;
    }
    return speed;
}

unsigned int cycle_colors(unsigned int color){
    if((color == COLOR_1)){
        color = COLOR_2;
    }else if((color == COLOR_2)){
        color = COLOR_3;
    }else if((color = COLOR_3)){
        color = COLOR_1;
    }
    return color;
}

bool play_again_screen(unsigned char *mem_base, unsigned char *parlcd_mem_base, unsigned short *fb){
    font_descriptor_t *fdes44 = &font_wArial_44; //  font template

    char c = '0';
    int option = 0;
    bool quit = false;
    bool newentry = true;
    uint32_t rgb_knobs_value;
    uint8_t pushedknob;
    while (!quit){
        if (kbhit()){
            c = getch();
        }
        rgb_knobs_value = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
        pushedknob = (uint8_t)(rgb_knobs_value >> 24);

        if (pushedknob == 0x04 || c == '-'){ //red knob || -
            if (option == 0){
                option = 1;
            }else{
                option = 0;
            }
            c = '0';
            newentry = true;
        }else if (pushedknob == 0x01 || c == '+'){ //blue knob || +
            if (option == 0){
                option = 1;
            }else{
                option = 0;
            }
            c = '0';
            newentry = true;
        }else if (pushedknob == 0x02 || c == 0x0A){ //green knob || enter
            c = '0';
            newentry = true;
            if (option == 0){
                return false;       // quit = false -> play again
            }else if (option == 1){
                return true;        // quit = true -> end
            }
        }

        if (newentry && !quit){
            newentry = false;
            switch (option)
            {
            case 0: // option 0 selected
                frame_buffer_clear(fb);
                draw_string(10, 160, "PLAY AGAIN", 10, BLUE_COLOR, fdes44, fb);     //option 0
                draw_string(350, 160, "END", 3, YELLOW_COLOR, fdes44, fb);          //option 1
                lcd_draw(parlcd_mem_base, fb);
                break;
            case 1: // option 1 selected
                frame_buffer_clear(fb);
                draw_string(10, 160, "PLAY AGAIN", 10, YELLOW_COLOR, fdes44, fb);   //option 0
                draw_string(350, 160, "END", 3, BLUE_COLOR, fdes44, fb);            //option 1
                lcd_draw(parlcd_mem_base, fb);
                break;
            default:
                break;
            }
        }
    }
    return false;
}
