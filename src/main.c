#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <file_fun.h>
#include <global.h>
#include <mega65/debug_calypis.h>
#include <chips.h>
#include <setup.h>


#define DEBUG

//#pragma require __preserve_zp

struct map_t __attribute__((zpage)) map_regs;
struct m65_dirent __attribute__((zpage)) *m65_dirent_exchange;

unsigned char __far *attic_memory;
unsigned char bank_file;

void loadresource(void);
void setup_resource(void);


void main(void) {


    loadresource();

    setup_resource();

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
