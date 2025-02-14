#include <sprite.h>
#include <chips.h>
#include <mega65/debug_calypis.h>

#define DEBUG

void create_sprite(struct _SPRITE *sprite, int x, unsigned char y, unsigned char spritenumber, unsigned char bit_Minus_X, unsigned char bit_Mag_X) {
    sprite->x = x;
    sprite->y = y;
    sprite->spritenumber = spritenumber;
    sprite->bit_Minus_X = bit_Minus_X;
    sprite->bit_Mag_X = bit_Mag_X;
}

void drawsprite(struct _SPRITE *sprite) {

    VIC2.S0Y = sprite->y;

    if (sprite->x <= 254) {
        VIC2.SXMSB = VIC2.SXMSB & sprite->bit_Minus_X; // Abilita il bit dello coordinate X <= 254 dello sprite
        VIC2.S0X = (unsigned char) sprite->x;
    }
    else {
        VIC2.SXMSB = VIC2.SXMSB | sprite->bit_Mag_X; // Abilita il bit dello coordinate X > 254 dello sprite
        VIC2.S0X = (unsigned char) (sprite->x -255);

    }

    #ifdef DEBUG
    debug_msg("DRAW SPRITE");
    char stringa[10];
	stringa[0]='Y';  
	stringa[1]=':';  
	stringa[6]='\0';
    itoa((unsigned int)sprite->y, stringa+2,10);
    debug_msg(stringa);
	stringa[0]='X';  
	stringa[1]=':';  
	stringa[6]='\0';
    itoa((unsigned int)sprite->x, stringa+2,10);
    debug_msg(stringa);  
	stringa[0]='B';  
	stringa[1]=':';  
	stringa[6]='\0';
    itoa((unsigned int)VIC2.SXMSB, stringa+2,10);
    debug_msg(stringa);  
    #endif  

}

void movesprite(struct _SPRITE *sprite, int x, char y) {

    sprite->x = sprite->x + x;
    sprite->y = sprite->y + y;

    #ifdef DEBUG
    debug_msg("MOVE SPRITE");
    char stringa[10];
	stringa[0]='Y';  
	stringa[1]=':';  
	stringa[6]='\0';
    itoa((int)sprite->y, stringa+2,10);
    debug_msg(stringa);
	stringa[0]='X';  
	stringa[1]=':';  
	stringa[6]='\0';
    itoa(sprite->x, stringa+2,10);
    debug_msg(stringa);
    #endif
}