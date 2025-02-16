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
    char Read_Sprite_Pos_Char_Y;
    char Read_Sprite_Pos_Char_X;
    char direction;
    unsigned char shape;
    unsigned char wall;
    unsigned char ladder;
    unsigned char collision;

};

void create_sprite(struct _SPRITE *sprite, int x, unsigned char y, unsigned char spritenumber, unsigned char bit_Minus_X, unsigned char bit_Mag_X);

void drawsprite(struct _SPRITE *sprite);

void movesprite(struct _SPRITE *sprite, int x, char y);
void movesprite_left(struct _SPRITE *sprite);
void movesprite_rigth(struct _SPRITE *sprite);
void movesprite_up(struct _SPRITE *sprite);
void movesprite_down(struct _SPRITE *sprite);
void check_fall_left(struct _SPRITE *sprite);
void check_fall_rigth(struct _SPRITE *sprite);



#endif