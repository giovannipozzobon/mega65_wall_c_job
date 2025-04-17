#include <debug_calypis.h>
#include <calypsi/intrinsics6502.h>
#include <gest_game.h>
#include <global.h>
#include "chips.h"



//#define DEBUG

int read_char(int x, int y){
    unsigned char c;

    c = FAR_U8_PTR(SCREEN_MAP) [y*80+x*2]; // ogni riga è di 80 caratteri e ogni carattere è di 2 byte

    #ifdef DEBUG
    char stringa [10];
    itoa((int) ((y*80)+(x*2)), stringa+2,10);
    stringa[0]='A';  
    stringa[1]=':';  
    stringa[8]='\0';
    debug_msg(stringa);

   
    itoa( c, stringa+2,10);
    stringa[0]='C';  
    stringa[1]=':';  
    stringa[6]='\0';
    debug_msg(stringa);
    
    #endif

    return c;

}

void delete_key(uint8_t Pos_Char_X, uint8_t Pos_Char_Y){
    FAR_U8_PTR(SCREEN_MAP) [Pos_Char_Y*80+Pos_Char_X*2] = SPACE;
    FAR_U8_PTR(SCREEN_MAP) [Pos_Char_Y*80+Pos_Char_X*2+1] = ((CHARSET_MEMORY / 64) >> 8);
}

void save_char_pos_enemy(Nemico *n){

    n->shape_old = FAR_U8_PTR(SCREEN_MAP) [n->posY*80+n->posX*2];

    #ifdef DEBUG
    char stringa [10];
    debug_msg("SAVE CHAR POS ENEMY");

    itoa((int) n->shape_old, stringa+2,10);
    stringa[0]='P';  
    stringa[1]=':';  
    stringa[8]='\0';
    debug_msg(stringa);
    #endif

}

void delete_enemy(Nemico *n){

    #ifdef DEBUG
    char stringa [10];
    debug_msg("DELETE ENEMY");

    itoa((int) Pos_Char_X, stringa+2,10);
    stringa[0]='A';  
    stringa[1]=':';  
    stringa[8]='\0';
    debug_msg(stringa);

   
    itoa( Pos_Char_Y, stringa+2,10);
    stringa[0]='C';  
    stringa[1]=':';  
    stringa[6]='\0';
    debug_msg(stringa);
    
    #endif

    FAR_U8_PTR(SCREEN_MAP) [n->PosCharY_old*80+n->PosCharX_old*2] = n->shape_old;
    FAR_U8_PTR(SCREEN_MAP) [n->PosCharY_old*80+n->PosCharX_old*2+1] = ((CHARSET_MEMORY / 64) >> 8);
}

void draw_enemy(Nemico *n){

    #ifdef DEBUG
    char stringa [10];
    debug_msg("DRAW ENEMY");

    itoa((int) Pos_Char_X, stringa+2,10);
    stringa[0]='A';  
    stringa[1]=':';  
    stringa[8]='\0';
    debug_msg(stringa);

   
    itoa( Pos_Char_Y, stringa+2,10);
    stringa[0]='C';  
    stringa[1]=':';  
    stringa[6]='\0';
    debug_msg(stringa);

    itoa( shape, stringa+2,10);
    stringa[0]='S';  
    stringa[1]=':';  
    stringa[6]='\0';
    debug_msg(stringa);
    #endif

    FAR_U8_PTR(SCREEN_MAP) [n->posY*80+n->posX*2] = n->shape;
    FAR_U8_PTR(SCREEN_MAP) [n->posY*80+n->posX*2+1] = ((CHARSET_MEMORY / 64) >> 8);
}