#include <sprite.h>
#include <chips.h>
#include <debug_calypis.h>
#include <global.h>
#include <func_job.h>
#include <gest_game.h>
#include <stdint.h>

//#define DEBUG
#define DEBUG_LIGTH


void create_sprite(Sprite *sprite, int sprite_number, int x, unsigned char y, unsigned char bit_Minus_X, unsigned char bit_Mag_X) {
    sprite->sprite_number = sprite_number;
    sprite->x = x;
    sprite->y = y;
    sprite->bit_Minus_X = bit_Minus_X;
    sprite->bit_Mag_X = bit_Mag_X;
    sprite->Read_Sprite_Pos_Char_X = (x/8)-3;
    sprite->Read_Sprite_Pos_Char_Y = (y/8)-3;
    sprite->direction = FALSE;
    sprite->wall = FALSE;
    sprite->ladder = FALSE;
    sprite->collision = FALSE;
    sprite->key_collected = 0;
    sprite->step_direction = 1;
}

void drawsprite(Sprite *sprite) {

    poke(0xd001+sprite->sprite_number, sprite->y);

    if (sprite->x <= 254) {
        poke(0xd000+sprite->sprite_number, sprite->x);
        //VIC2.S0X = sprite->x;
        VIC2.SXMSB = VIC2.SXMSB & sprite->bit_Minus_X; // Abilita il bit dello coordinate X <= 254 dello sprite
        #ifdef DEBUG_LIGTH
        debug_msg("DRAW SPRITE SXMSB MIN");
        #endif

    }
    else {
        poke(0xd000+sprite->sprite_number, sprite->x-255);
        //VIC2.S0X = sprite->x-255;
        VIC2.SXMSB = VIC2.SXMSB | sprite->bit_Mag_X; // Abilita il bit dello coordinate X > 254 dello sprite
        #ifdef DEBUG_LIGTH
        debug_msg("DRAW SPRITE SXMSB MAG");
        #endif

    }
    

    #ifdef DEBUG
    debug_msg("DRAW SPRITE");
    char stringa[10];
	stringa[0]='S';  
	stringa[1]=':';  
	stringa[6]='\0';
    itoa(0xd000+sprite->sprite_number, stringa+2,16);
    debug_msg(stringa);
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

void drawsprite_monster(Sprite *sprite) {

    poke(0xd000+(sprite->sprite_number*2)+1, sprite->y);

    if (sprite->x <= 254) {
        poke(0xd000+(sprite->sprite_number*2), sprite->x);
        //VIC2.S0X = sprite->x;
        VIC2.SXMSB = VIC2.SXMSB & sprite->bit_Minus_X; // Abilita il bit dello coordinate X <= 254 dello sprite

    }
    else {
        poke(0xd000+(sprite->sprite_number*2), sprite->x-255);
        //VIC2.S0X = sprite->x-255;
        VIC2.SXMSB = VIC2.SXMSB | sprite->bit_Mag_X; // Abilita il bit dello coordinate X > 254 dello sprite

    }

/*
poke(0xd003, sprite->y);

if (sprite->x <= 254) {

    //VIC2.S0X = sprite->x;
    VIC2.SXMSB = VIC2.SXMSB & sprite->bit_Minus_X; // Abilita il bit dello coordinate X <= 254 dello sprite 
    poke(0xd002, sprite->x);
    #ifdef DEBUG_LIGTH
    debug_msg("DRAW MONSTER SXMSB MIN");
    #endif

}
else {
    poke(0xd002, sprite->x-255);
    //VIC2.S0X = sprite->x-255;
    VIC2.SXMSB = VIC2.SXMSB | sprite->bit_Mag_X; // Abilita il bit dello coordinate X > 254 dello sprite
    #ifdef DEBUG_LIGTH
    debug_msg("DRAW MONSTER SXMSB MAG");
    #endif

}
*/

#ifdef DEBUG
char stringa[10];
stringa[0]='H';  
stringa[1]=':';  
stringa[6]='\0';
itoa(sprite->bit_Minus_X, stringa+2,10);
debug_msg(stringa);
#endif


wait_raster(DELAY_JOY);

}


void check_fall_left(Sprite *sprite){ 
    int Char;

    #ifdef DEBUG_LIGTH
    debug_msg("CHECK FALL LEFT");
    #endif

    while (1)
    {
        //Char = read_char(((sprite->x+6)/8)-3, ((sprite->y+19)/8)-6); // verifica se serve aggiungere 1 al posizione X del carattere
        Char = read_char(((sprite->x+2)/8)-2, ((sprite->y+16)/8)-3);

        #ifdef DEBUG
        char stringa[10];
        stringa[0]='H';  
        stringa[1]=':';  
        stringa[6]='\0';
        itoa((int)Char, stringa+2,10);
        debug_msg(stringa);
        #endif
        
        if (Char == SPACE) {
            sprite->y++;
            drawsprite(sprite);
            #ifdef DEBUG_LIGTH
            debug_msg("FALL LEFT OK");
            #endif
        }   else {
            #ifdef DEBUG_LIGTH
            debug_msg("CHECK FALL LEFT EXIT");
            #endif
           return;
        }
    }

}

void check_fall_rigth(Sprite *sprite){ 
    int Char, Char1;

    #ifdef DEBUG_LIGTH
    debug_msg("CHECK FALL RIGTH");
    #endif

    while (1)
    {
        //Char = read_char(((sprite->x+2)/8)-3, ((sprite->y+19)/8)-6); // verifica se serve aggiungere 1 al posizione X del carattere
        Char = read_char(((sprite->x+2)/8)-2, ((sprite->y+16)/8)-3);
        Char1 =read_char(((sprite->x+2)/8)-3, ((sprite->y+16)/8)-3);

        #ifdef DEBUG
        char stringa[10];
        stringa[0]='G';  
        stringa[1]=':';  
        stringa[6]='\0';
        itoa(Char, stringa+2,10);
        debug_msg(stringa);
        #endif
        
        if ((Char == SPACE) && (Char1 == SPACE)){
            sprite->y++;
            drawsprite(sprite);
            #ifdef DEBUG_LIGTH
            debug_msg("FALL RIGTH OK");
            #endif
        }   else {
            #ifdef DEBUG_LIGTH
            debug_msg("CHECK FALL RIGTH EXIT");
            #endif
            return;
        }
    }

}

void movesprite(Sprite *sprite, int x, char y) {

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

void movesprite_left(Sprite *sprite){
    int Char;
    
    //Check coordinate x and collision
    if ((sprite->collision == FALSE) &&(sprite->x <= 24)) return;

    // reset variables  
    sprite->wall = FALSE;
    sprite->ladder = FALSE;
    sprite->key = FALSE;
    sprite->door = FALSE;
    
    //calculate new position and char position
    sprite->x--;
    sprite->Read_Sprite_Pos_Char_X = (sprite->x/8)-3;
    sprite->Read_Sprite_Pos_Char_Y = ((sprite->y/8)-4) > 0  ? ((sprite->y/8)-4) : 0 ;
    sprite->direction = LEFT;


    //debug information
    #ifdef DEBUG
    debug_msg("LEFT");
    debug_msg("MOVE SPRITE LEFT");
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

    stringa[0]='R';  
	stringa[1]=':';  
	stringa[6]='\0';
    itoa((int)sprite->Read_Sprite_Pos_Char_X, stringa+2,10);
    debug_msg(stringa);
	stringa[0]='R';  
	stringa[1]=':';  
	stringa[6]='\0';
    itoa(sprite->Read_Sprite_Pos_Char_Y, stringa+2,10);
    debug_msg(stringa);
    #endif

    // todo: sprite change shape


    
    // check wall routine
    Char = read_char(sprite->Read_Sprite_Pos_Char_X, sprite->Read_Sprite_Pos_Char_Y);
    if (Char == WALL) {
        sprite->wall = TRUE;
        #ifdef DEBUG_LIGTH
        debug_msg(" MOVE LEFT - WALL 1");
        #endif        
    } else if (Char == LADDER){
        sprite->ladder = TRUE;
        #ifdef DEBUG_LIGTH
        debug_msg(" MOVE LEFT - LADDER 1");
        #endif  
    } else if (Char == KEYS){
        sprite->key = TRUE;
        #ifdef DEBUG_LIGTH
        debug_msg(" MOVE LEFT - KEY 1");
        #endif  
        sprite->key_collected++;
        delete_key(sprite->Read_Sprite_Pos_Char_X, sprite->Read_Sprite_Pos_Char_Y);
    } else if (Char == DOORS){
        sprite->door = TRUE;
        #ifdef DEBUG_LIGTH
        debug_msg(" MOVE LEFT - DOOR 1");
        #endif  
    } 

    // check fall routine
    check_fall_left(sprite);

    #ifdef DEBUG
    stringa[0]='L';  
    stringa[1]=':';
    stringa[2]=' ';
    stringa[3]=' ';
    stringa[4]=' ';      
    stringa[6]='\0';
    itoa(Char, stringa+2,10);
    debug_msg(stringa);
    #endif

    Char = read_char(sprite->Read_Sprite_Pos_Char_X+1, sprite->Read_Sprite_Pos_Char_Y+1);
    if (Char == WALL) {
        sprite->wall = TRUE;
        #ifdef DEBUG_LIGTH
        debug_msg(" MOVE LEFT - WALL 2");
        #endif        
    } else if (Char == LADDER){
        sprite->ladder = TRUE;
        #ifdef DEBUG_LIGTH
        debug_msg(" MOVE LEFT - LADDER 2");
        #endif  
    } else if (Char == KEYS){
        sprite->key = TRUE;
        #ifdef DEBUG_LIGTH
        debug_msg(" MOVE LEFT - KEY 2");
        #endif  
        sprite->key_collected++;
        delete_key(sprite->Read_Sprite_Pos_Char_X+1, sprite->Read_Sprite_Pos_Char_Y+1);
    } else if (Char == DOORS){
        sprite->door = TRUE;
        #ifdef DEBUG_LIGTH
        debug_msg(" MOVE LEFT - DOOR 2");
        #endif  
    } 

    #ifdef DEBUG
    stringa[0]='L';  
    stringa[1]=':';
    stringa[2]=' ';
    stringa[3]=' ';
    stringa[4]=' ';      
    stringa[6]='\0';
    itoa(Char, stringa+2,10);
    debug_msg(stringa);
    #endif

    Char = read_char(sprite->Read_Sprite_Pos_Char_X+1, sprite->Read_Sprite_Pos_Char_Y+2);
    if (Char == WALL) {
        sprite->wall = TRUE;
        #ifdef DEBUG_LIGTH
        debug_msg(" MOVE LEFT - WALL 3");
        #endif        
    } else if (Char == LADDER){
        sprite->ladder = TRUE;
        #ifdef DEBUG_LIGTH
        debug_msg(" MOVE LEFT - LADDER 3");
        #endif  
    } else if (Char == KEYS){
        sprite->key = TRUE;
        #ifdef DEBUG_LIGTH
        debug_msg(" MOVE LEFT - KEY 3");
        #endif  
        sprite->key_collected++;
        delete_key(sprite->Read_Sprite_Pos_Char_X+1, sprite->Read_Sprite_Pos_Char_Y+2);
    } else if (Char == DOORS){
        sprite->door = TRUE;
        #ifdef DEBUG_LIGTH
        debug_msg(" MOVE LEFT - DOOR 3");
        #endif  
    } 

    #ifdef DEBUG
    stringa[0]='L';  
    stringa[1]=':';
    stringa[2]=' ';
    stringa[3]=' ';
    stringa[4]=' ';      
    stringa[6]='\0';
    itoa(Char, stringa+2,10);
    debug_msg(stringa);
    #endif
    
    // todo: check detect collect routine

    if (sprite->wall == TRUE){
        sprite->x++;
        #ifdef DEBUG
        debug_msg(" MOVE LEFT - WALL X++");
        stringa[0]='W';  
        stringa[1]=':';    
        stringa[6]='\0';
        itoa(sprite->wall, stringa+2,10);
        debug_msg(stringa);
        #endif        
    }   


    if (sprite->door == TRUE) {
        if (sprite->key_collected > 0) {
            #ifdef DEBUG_LIGTH
            debug_msg("OPEN DOOR");
            #endif
        } else{
            #ifdef DEBUG_LIGTH
            debug_msg("DOOR IS CLOSED");
            #endif
            sprite->door = FALSE;
        }
    }


}

void movesprite_rigth(Sprite *sprite){
    int Char;

    //Check coordinate x and collision
    if ((sprite->collision == FALSE) &&(sprite->x > 320)) return; //todo: check value 312 è correttto?

    // reset variables  
    sprite->wall = FALSE;
    sprite->ladder = FALSE;
    sprite->key = FALSE;
    sprite->door = FALSE;

    //calculate new position and char position
    sprite->x++;
    //sprite->Read_Sprite_Pos_Char_X = (sprite->x/8)-3;
    sprite->Read_Sprite_Pos_Char_X = (sprite->x/8)-3;
    //sprite->Read_Sprite_Pos_Char_Y = ((sprite->y/8)-6) > 0  ? ((sprite->y/8)-6) : 0 ;
    sprite->Read_Sprite_Pos_Char_Y = ((sprite->y/8)-4) > 0  ? ((sprite->y/8)-4) : 0 ;
    sprite->direction = RIGTH;



    #ifdef DEBUG
    debug_msg("RIGTH");
    debug_msg("MOVE SPRITE RIGTH");
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

    stringa[0]='R';  
	stringa[1]=':';  
	stringa[6]='\0';
    itoa((int)sprite->Read_Sprite_Pos_Char_X, stringa+2,10);
    debug_msg(stringa);
	stringa[0]='R';  
	stringa[1]=':';  
	stringa[6]='\0';
    itoa(sprite->Read_Sprite_Pos_Char_Y, stringa+2,10);
    debug_msg(stringa);
    #endif

    // todo: sprite change shape

    
    // check collision routine
    Char = read_char(sprite->Read_Sprite_Pos_Char_X, sprite->Read_Sprite_Pos_Char_Y);
    if (Char == WALL) {
        sprite->wall = TRUE;
        #ifdef DEBUG_LIGTH
        debug_msg(" MOVE RIGTH - WALL 1");
        #endif        
    } else if (Char == LADDER){
        sprite->ladder = TRUE;
        #ifdef DEBUG_LIGTH
        debug_msg(" MOVE RIGTH - LADDER 1");
        #endif  
    } else if (Char == KEYS){
        sprite->key = TRUE;
        #ifdef DEBUG_LIGTH
        debug_msg(" MOVE RIGTH - KEY 1");
        #endif  
        sprite->key_collected++;
        delete_key(sprite->Read_Sprite_Pos_Char_X, sprite->Read_Sprite_Pos_Char_Y);
    } else if (Char == DOORS){
        sprite->door = TRUE;
        #ifdef DEBUG_LIGTH
        debug_msg(" MOVE RIGTH - DOOR 1");
        #endif  
    } 

    #ifdef DEBUG
    stringa[0]='Q';  
    stringa[1]=':';
    stringa[2]=' ';
    stringa[3]=' ';
    stringa[4]=' ';      
    stringa[6]='\0';
    itoa(Char, stringa+2,10);
    debug_msg(stringa);
    #endif

    // check fall routine
    check_fall_rigth(sprite);

    Char = read_char(sprite->Read_Sprite_Pos_Char_X+1, sprite->Read_Sprite_Pos_Char_Y+1);
    if (Char == WALL) {
        sprite->wall = TRUE;
        #ifdef DEBUG_LIGTH
        debug_msg(" MOVE RIGTH - WALL 2");
        #endif        
    } else if (Char == LADDER){
        sprite->ladder = TRUE;
        #ifdef DEBUG_LIGTH
        debug_msg(" MOVE RIGTH - LADDER 2");
        #endif  
    } else if (Char == KEYS){
        sprite->key = TRUE;
        #ifdef DEBUG_LIGTH
        debug_msg(" MOVE RIGTH - KEY 2");
        #endif  
        sprite->key_collected++;
        delete_key(sprite->Read_Sprite_Pos_Char_X+1, sprite->Read_Sprite_Pos_Char_Y+1);
    } else if (Char == DOORS){
        sprite->door = TRUE;
        #ifdef DEBUG_LIGTH
        debug_msg(" MOVE RIGTH - DOOR 2");
        #endif  
    } 

    #ifdef DEBUG
    stringa[0]='Q';  
    stringa[1]=':';
    stringa[2]=' ';
    stringa[3]=' ';
    stringa[4]=' ';      
    stringa[6]='\0';
    itoa(Char, stringa+2,10);
    debug_msg(stringa);
    #endif

    Char = read_char(sprite->Read_Sprite_Pos_Char_X+1, sprite->Read_Sprite_Pos_Char_Y+2);
    if (Char == WALL) {
        sprite->wall = TRUE;
        #ifdef DEBUG_LIGTH
        debug_msg(" MOVE RIGTH - WALL 3");
        #endif        
    } else if (Char == LADDER){
        sprite->ladder = TRUE;
        #ifdef DEBUG_LIGTH
        debug_msg(" MOVE RIGTH - LADDER 3");
        #endif  
    } else if (Char == KEYS){
        sprite->key = TRUE;
        #ifdef DEBUG_LIGTH
        debug_msg(" MOVE RIGTH - KEY 3");
        #endif 
        sprite->key_collected++;
        delete_key(sprite->Read_Sprite_Pos_Char_X+1, sprite->Read_Sprite_Pos_Char_Y+2);
    } else if (Char == DOORS){
        sprite->door = TRUE;
        #ifdef DEBUG_LIGTH
        debug_msg(" MOVE RIGTH - DOOR 3");
        #endif  
    }   
    

    #ifdef DEBUG
    stringa[0]='Q';  
    stringa[1]=':';
    stringa[2]=' ';
    stringa[3]=' ';
    stringa[4]=' ';      
    stringa[6]='\0';
    itoa(Char, stringa+2,10);
    debug_msg(stringa);
    #endif

    if (sprite->wall == TRUE) {
        sprite->x--;
        #ifdef DEBUG
        debug_msg(" MOVE RIGTH - WALL X--");
        stringa[0]='W';  
        stringa[1]=':';    
        stringa[6]='\0';
        itoa(sprite->wall, stringa+2,10);
        debug_msg(stringa);
        #endif        
    }    

    if (sprite->door == TRUE) {
        if (sprite->key_collected > 0) {
            #ifdef DEBUG_LIGTH
            debug_msg("OPEN DOOR");
            #endif
        } else{
            #ifdef DEBUG_LIGTH
            debug_msg("DOOR IS CLOSED");
            #endif
            sprite->door = FALSE;
        }
    }

    
}

void movesprite_up(Sprite *sprite){
    int Char;

    // reset variables
    sprite->ladder = FALSE;

    //check if sprite is on ladder
    Char = read_char(((sprite->x+4)/8)-3, ((sprite->y+8)/8)-3);
    if (Char == LADDER) sprite->ladder = TRUE;
    Char = read_char(((sprite->x+4)/8)-3, ((sprite->y+14)/8)-3);
    if (Char == LADDER) sprite->ladder = TRUE;    
    
    if (sprite->ladder == FALSE) return;

    sprite->y--;

}

void movesprite_down(Sprite *sprite){
    int Char;

    // reset variables
    sprite->ladder = FALSE;

    //check if sprite is on ladder
    //Char = read_char(((sprite->x+4)/8)-3, ((sprite->y+8)/8)-3);
    //if (Char == LADDER) sprite->ladder = TRUE;
    Char = read_char(((sprite->x+4)/8)-3, ((sprite->y+16)/8)-3);
    if (Char == LADDER) sprite->ladder = TRUE;    
    
    if (sprite->ladder == FALSE) return;

    sprite->y++;

 
}

void jumpsprite(Sprite *sprite){
    int count = 0;

    //Check if it can jump
    if (sprite->ladder == TRUE) return;
    
    //sprite->jumping = 0;

    while (count  < JUMP_LEN)
    {
        sprite->y = sprite->y+spread_vert[count];
        drawsprite(sprite);
        wait_very_briefly(DELAY_JUMP);
        //wait_raster(DELAY_JUMP);
        count++;

        #ifdef DEBUG   
        debug_msg("JUMP");
        char stringa[10];
        stringa[0]='J';  
        stringa[1]=':';  
        stringa[6]='\0';
        itoa((int)sprite->y, stringa+2,10);
        debug_msg(stringa);
        #endif
    }
}      

void jumpsprite_rigth(Sprite *sprite){
    int count = 0;
    int Char;

    //Check if it can jump
    if (sprite->ladder == TRUE) return;
    
    //sprite->jumping = 0;

    while (count  < JUMP_LEN)
    {
        sprite->y = sprite->y+spread_vert[count];
        sprite->x = sprite->x+spread_horiz[count];
        drawsprite(sprite);
        wait_very_briefly(DELAY_JUMP);
        //wait_raster(DELAY_JUMP);
        count++;

        #ifdef DEBUG   
        debug_msg("JUMP RIGTH");
        char stringa[10];
        stringa[0]='J';  
        stringa[1]=':';  
        stringa[6]='\0';
        itoa((int)sprite->y, stringa+2,10);
        debug_msg(stringa);
        #endif

        //check if the sprite reachs the floor
        Char = read_char(((sprite->x+2)/8)-2, ((sprite->y+16)/8)-3);
        if (Char == WALL) {
            #ifdef DEBUG_LIGTH   
            debug_msg("EXIT JUMP RIGTH REACH WALL");
            #endif           
            return;
        }

    }

    //check if the sprite doesn't reach the floor
    check_fall_rigth(sprite);

}      


void jumpsprite_left(Sprite *sprite){
    int count = 0;
    int Char;

    //Check if it can jump
    if (sprite->ladder == TRUE) return;
    
    //sprite->jumping = 0;

    while (count  < JUMP_LEN)
    {
        sprite->y = sprite->y+spread_vert[count];
        sprite->x = sprite->x-spread_horiz[count];
        drawsprite(sprite);
        wait_very_briefly(DELAY_JUMP);
        //wait_raster(DELAY_JUMP);
        count++;

        #ifdef DEBUG   
        debug_msg("JUMP LEFT");
        char stringa[10];
        stringa[0]='J';  
        stringa[1]=':';  
        stringa[6]='\0';
        itoa((int)sprite->y, stringa+2,10);
        debug_msg(stringa);
        #endif

        Char = read_char(((sprite->x+2)/8)-2, ((sprite->y+16)/8)-3);
        if (Char == WALL) {
            #ifdef DEBUG_LIGTH   
            debug_msg("EXIT JUMP LEFT REACH WALL");
            #endif

            return;    
            
        }

    }
    //check if the sprite doesn't reach the floor
    check_fall_left(sprite);
}    