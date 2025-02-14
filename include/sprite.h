#ifndef __SPRITE_H
#define __SPRITE_H

struct _SPRITE {
    unsigned int x;
    unsigned char y;
    unsigned char upper_254_x;
    unsigned char spritenumber;
    unsigned char enabled;
    unsigned char bit_Minus_X;
    unsigned char bit_Mag_X;
};

void create_sprite(struct _SPRITE *sprite, int x, unsigned char y, unsigned char spritenumber, unsigned char bit_Minus_X, unsigned char bit_Mag_X);

void drawsprite(struct _SPRITE *sprite);

void movesprite(struct _SPRITE *sprite, int x, char y);

#endif