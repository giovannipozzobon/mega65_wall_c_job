#ifndef __SPRITE_H
#define __SPRITE_H

#include <stdint.h>

struct _SPRITE {
    uint16_t sprite_number;
    uint16_t x;
    uint8_t y;
    uint8_t upper_254_x;
    uint8_t enabled;
    uint8_t bit_Minus_X;
    uint8_t bit_Mag_X;
    uint8_t Read_Sprite_Pos_Char_Y;
    uint8_t Read_Sprite_Pos_Char_X;
    uint8_t direction;
    uint8_t shape;
    uint8_t wall;
    uint8_t ladder;
    uint8_t key;
    uint8_t door;
    uint8_t collision;
    uint16_t jumping;
    uint8_t key_collected;

};

// Array of jump spread 
char spread_horiz[]= {0,4,4,4,4,0};
char spread_vert[]= {-8,-8,0,0,8,8};

void create_sprite(struct _SPRITE *sprite, int sprite_number, int x, unsigned char y, unsigned char bit_Minus_X, unsigned char bit_Mag_X);

void drawsprite(struct _SPRITE *sprite);

void movesprite(struct _SPRITE *sprite, int x, char y);
void movesprite_left(struct _SPRITE *sprite);
void movesprite_rigth(struct _SPRITE *sprite);
void movesprite_up(struct _SPRITE *sprite);
void movesprite_down(struct _SPRITE *sprite);
void check_fall_left(struct _SPRITE *sprite);
void check_fall_rigth(struct _SPRITE *sprite);
void jumpsprite(struct _SPRITE *sprite);



#endif