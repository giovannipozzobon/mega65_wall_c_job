#ifndef __GEST_GAME_H
#define __GEST_GAME_H

#include <stdint.h>
#include <sprite.h>

/*
unsigned char wr = 0; // wall rigth
unsigned char wl = 0; // wall left
unsigned char wu = 0; // wall up
unsigned char wd = 0; // wall down

unsigned char hk = 0; // hit key

unsigned char lu = 0; //ladder up
unsigned char ld = 0; //ladder down

*/

int read_char(int x, int y);

void delete_key(uint8_t Pos_Char_X, uint8_t Pos_Char_Y);
void delete_enemy(Nemico *n);
void draw_enemy(Nemico *n);
void save_char_pos_enemy(Nemico *n);

#endif