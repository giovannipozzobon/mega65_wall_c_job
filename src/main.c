#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <file_fun.h>
#include <global.h>
#include <mega65/debug_calypis.h>
#include <chips.h>
#include <setup.h>
#include <input.h>
#include <func_job.h>


#define DEBUG

#pragma require __preserve_zp

struct map_t __attribute__((zpage)) map_regs;
struct m65_dirent __attribute__((zpage)) *m65_dirent_exchange;

unsigned char __far *attic_memory;
unsigned char bank_file;

void loadresource(void);
void setup_resource(void);
void gameloop(void);


void main(void) {


    loadresource();

    setup_resource();

    while(1) {
        wait_very_briefly(DELAY_JOY);
        gameloop();
    }

    return;

}

void gameloop(void) {

       
    process_input();

    // left fire?
    if (player_input == 0b00010100)  {   
        // move left fire
        #ifdef DEBUG
        debug_msg("JOY LEFT FIRE");
        #endif

        return;
    }

    // rigth fire?
    if (player_input == 0b00011000)  {   
        // move rigth fire
        #ifdef DEBUG
        debug_msg("JOY RIGTH FIRE");
        #endif

        return;
    }

    // left?
    if (player_input == 0b00000100)  {   
        // move left
        #ifdef DEBUG
        debug_msg("JOY LEFT");
        #endif

        return;
    }
    // right?
    if (player_input == 0b00001000) {
        // move right
        #ifdef DEBUG
        debug_msg("JOY RIGTH");
        #endif

        return;
    }
    // up?
    if (player_input == 0b00000001) {
        // move up
        #ifdef DEBUG
        debug_msg("JOY UP");
        #endif

        return;
    }
    // down?
    if (player_input == 0b00000010) {
        // move down
        #ifdef DEBUG
        debug_msg("JOY DOWN");
        #endif

        return;
    }
    // fire?
    if (player_input == 0b00010000) {
        // fire
        #ifdef DEBUG
        debug_msg("JOY FIRE ");
        #endif

        return;
    }       

    return;
}

void loadresource(void) {
    
    //load the charset file
    bank_file = ATTIC_MEM;
    attic_memory = attic_memory_charset;
    loadfile(charfile);

    //load the screen file
    attic_memory = attic_memory_screen;
    loadfile(screenfile);

    //load the sprite file
    attic_memory = attic_memory_sprite;
    loadfile(spritefile);

    return;
}

void setup_resource(void){

    initial_setup();

    setchar(attic_memory_charset);

    colorset_init();   
    setscreen(attic_memory_screen);
    setsprite(attic_memory_sprite);
    
    screen_setup();

    // prepare sprite
    sprite_setup();

    return;
}
