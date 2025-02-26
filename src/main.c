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

//#define DEBUG


#pragma require __preserve_zp

struct map_t __attribute__((zpage)) map_regs;
struct m65_dirent __attribute__((zpage)) *m65_dirent_exchange;

unsigned char __far *attic_memory;
unsigned char bank_file;

uint8_t level;

Sprite  * sprite, * sprite_monster;

void loadresource(void);
void setup_resource(void);
void gameloop(void);
void load_screen(uint8_t level);
void move_monsters(void);



void main(void) {

    //alloc memory for Sprite array 
    sprite = malloc(sizeof(Sprite));
    sprite_monster = malloc(sizeof(Sprite));

    //level 0 (first level)
    level = 0;

    loadresource();

    setup_resource();


    // Create Sprite 0
	create_sprite(&sprite[0], 0, 0x20, 0x45,0b11111110,0b00000001);
	//drawsprite(&sprite[0]);

	// Create Sprite 1
	//create_sprite(&sprite[1], 1, 0x80, 0x80,0b11111101,0b00000010); 
    create_sprite(sprite_monster, 1, 0x80, 0x80,0b11111101,0b00000010);
	//drawsprite(&sprite[1]);


    while(1) {
        //wait_very_briefly(DELAY_JOY);
        wait_raster(DELAY_JOY);
        gameloop();
        
        if (sprite->door == TRUE) {
            #ifdef DEBUG
            debug_msg("LOAD NEW LEVEL ");
            #endif
            level++;
            load_screen(level);
        }

        move_monsters();
    }

    free(sprite);
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

    // key N?  Load new level
    if (player_input == 0b00100000) {
        // fire
        #ifdef DEBUG
        debug_msg("LOAD NEW LEVEL ");
        #endif
        level++;
        load_screen(level);
        return;
    }  

    return;
}

void move_monsters (void) {

    if (sprite_monster->x > 320){

        sprite_monster->step_direction = -1;
    } 
    if (sprite_monster->x <= 24){

        sprite_monster->step_direction = 1;
    }   

    sprite_monster->x = sprite_monster->x + sprite_monster->step_direction;

    drawsprite_monster(sprite_monster);

}

void loadresource(void) {
    
    //load the charset file
    bank_file = ATTIC_MEM;
    attic_memory = attic_memory_charset;
    loadfile(charfile);

    //load the screen file 0
    attic_memory = attic_memory_screen;
    loadfile(screenfile);

    //load the screen file 1
    attic_memory = attic_memory_screen+(STEP_MEMORY_SCREEN*1);
    loadfile(screenfile1);

    //load the screen file 2
    attic_memory = attic_memory_screen+(STEP_MEMORY_SCREEN*2);
    loadfile(screenfile);

    //load the sprite file
    attic_memory = attic_memory_sprite;
    loadfile(spritefile);

    return;
}

void load_screen(uint8_t level) {
    //load the screen file

    // todo: togliere serve solo per fare un test
    level = 1;
    #ifdef DEBUG
    debug_msg("LOAD NEXT SCREEN");
    char stringa[10];
	stringa[0]='S';  
	stringa[1]=':';  
	stringa[6]='\0';
    itoa((uint16_t) attic_memory_screen+(STEP_MEMORY_SCREEN*level), stringa+2,16);
    debug_msg(stringa);

    #endif
    setscreen(attic_memory_screen+(STEP_MEMORY_SCREEN*level));

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
