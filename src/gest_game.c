#include <debug_calypis.h>
#include <calypsi/intrinsics6502.h>
#include <gest_game.h>
#include <global.h>
#include "chips.h"


//#define DEBUG

int read_char(int x, int y){
    char stringa [10];
    unsigned char c;

    c = FAR_U8_PTR(SCREEN_MAP) [y*80+x*2]; // ogni riga è di 80 caratteri e ogni carattere è di 2 byte

    #ifdef DEBUG
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

