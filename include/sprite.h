#ifndef __SPRITE_H
#define __SPRITE_H

#include <stdint.h>
#include <stdbool.h>

#define NUM_WAYPOINTS 4 // Numero di punti nel percorso

// Struttura per un punto del percorso (waypoint)
typedef struct {
    int x;
    int y;
} Waypoint;

typedef struct {
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
    uint16_t step_direction;

} Sprite;

// Type of enemy
typedef enum { LINEARE, WAYPOINT, AI } TypeEnemy;

// Stati del nemico
typedef enum { PATROL, CHASE, RETURN } StatoNemico;

// Struttura del nemico
typedef struct {
    uint16_t sprite_number;
    uint8_t posX;
    uint8_t posY;
    uint8_t velX;
    uint16_t velY;
    uint16_t limiteSinistro;
    uint16_t limiteDestro;
    uint16_t distanzaMinima;
    StatoNemico stato;
    uint8_t shape;
    uint8_t shape_old;
    uint8_t PosCharX_old;
    uint8_t PosCharY_old;
    uint8_t type;
    uint8_t targetIndex;
    bool avanti;
} Nemico;

// perscorso nemico waypoint
Waypoint percorso[NUM_WAYPOINTS] = {
    {10, 5}, {20, 5}, {30, 10}, {20, 15}  // Il nemico seguirà questi punti
};

// Array of jump spread 
char spread_horiz[]= {0,4,4,4,4,0};
char spread_vert[]= {-8,-8,0,0,8,8};

void create_sprite(Sprite *sprite, int sprite_number, int x, unsigned char y, unsigned char bit_Minus_X, unsigned char bit_Mag_X);

void crea_nemico(Nemico *nemico, uint16_t sprite_number, uint8_t x, uint8_t y, uint8_t velX, uint8_t velY, uint16_t limiteSinistro, uint16_t limiteDestro, uint16_t distanzaMinima, StatoNemico stato, TypeEnemy type);

void drawsprite(Sprite *sprite);
void drawnemico(Nemico *nemico);

void movesprite(Sprite *sprite, int x, char y);
void movesprite_left(Sprite *sprite);
void movesprite_rigth(Sprite *sprite);
void movesprite_up(Sprite *sprite);
void movesprite_down(Sprite *sprite);
void check_fall_left(Sprite *sprite);
void check_fall_rigth(Sprite *sprite);
void jumpsprite(Sprite *sprite);



#endif