#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <file_fun.h>
#include <global.h>
#include <debug_calypis.h>
#include <chips.h>
#include <setup.h>
#include <input.h>
#include <func_job.h>
#include <sprite.h>

#define DEBUG


#pragma require __preserve_zp

struct _SPRITE sprite[8]; 

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


    // Create Sprite 0
	create_sprite(&sprite[0], 0, 0x20, 0x45,0b11111110,0b00000001);
	//drawsprite(&sprite[0]);

	// Create Sprite 1
	create_sprite(&sprite[1], 1, 0x80, 0x80,0b11111101,0b00000010);
	//drawsprite(&sprite[1]);


    while(1) {
        //wait_very_briefly(DELAY_JOY);
        wait_raster(DELAY_JOY);
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
        jumpsprite_left(&sprite[0]);
        drawsprite(&sprite[0]);
        return;
    }

    // rigth fire?
    if (player_input == 0b00011000)  {   
        // move rigth fire
        #ifdef DEBUG
        debug_msg("JOY RIGTH FIRE");
        #endif
        jumpsprite_rigth(&sprite[0]);
        drawsprite(&sprite[0]);
        return;
    }

    // left?
    if (player_input == 0b00000100)  {   
        // move left
        #ifdef DEBUG
        debug_msg("JOY LEFT");
        #endif

        movesprite_left(&sprite[0]);
        drawsprite(&sprite[0]);
        return;
    }
    // right?
    if (player_input == 0b00001000) {
        // move right
        #ifdef DEBUG
        debug_msg("JOY RIGTH");
        #endif

        movesprite_rigth(&sprite[0]);
        drawsprite(&sprite[0]);
        return;
    }
    // up?
    if (player_input == 0b00000001) {
        // move up
        #ifdef DEBUG
        debug_msg("JOY UP");
        #endif

        movesprite_up(&sprite[0]);
        drawsprite(&sprite[0]);
        return;
    }
    // down?
    if (player_input == 0b00000010) {
        // move down
        #ifdef DEBUG
        debug_msg("JOY DOWN");
        #endif

        movesprite_down(&sprite[0]);
        drawsprite(&sprite[0]);
        return;
    }
    // fire?
    if (player_input == 0b00010000) {
        // fire
        #ifdef DEBUG
        debug_msg("JOY FIRE ");
        #endif
        jumpsprite(&sprite[0]);
        //drawsprite(&sprite[0]);
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
