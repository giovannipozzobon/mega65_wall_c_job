#include <sprite.h>
#include <chips.h>
#include <debug_calypis.h>
#include <global.h>
#include <func_job.h>

//#define DEBUG


void create_sprite(struct _SPRITE *sprite, int x, unsigned char y, unsigned char spritenumber, unsigned char bit_Minus_X, unsigned char bit_Mag_X) {
    sprite->x = x;
    sprite->y = y;
    sprite->spritenumber = spritenumber;
    sprite->bit_Minus_X = bit_Minus_X;
    sprite->bit_Mag_X = bit_Mag_X;
    sprite->Read_Sprite_Pos_Char_X = (x/8)-3;
    sprite->Read_Sprite_Pos_Char_Y = (y/8)-3;
    sprite->direction = FALSE;
    sprite->wall = FALSE;
    sprite->ladder = FALSE;
    sprite->collision = FALSE;
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

void check_fall_left(struct _SPRITE *sprite){ 
    int Char;

    #ifdef DEBUG
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
            #ifdef DEBUG
            debug_msg("FALL LEFT OK");
            #endif
        }   else {
            #ifdef DEBUG
            debug_msg("CHECK FALL LEFT EXIT");
            #endif
           return;
        }
    }

}

void check_fall_rigth(struct _SPRITE *sprite){ 
    int Char;

    #ifdef DEBUG
    debug_msg("CHECK FALL RIGTH");
    #endif

    while (1)
    {
        //Char = read_char(((sprite->x+2)/8)-3, ((sprite->y+19)/8)-6); // verifica se serve aggiungere 1 al posizione X del carattere
        Char = read_char(((sprite->x+2)/8)-3, ((sprite->y+16)/8)-3);

        #ifdef DEBUG
        char stringa[10];
        stringa[0]='G';  
        stringa[1]=':';  
        stringa[6]='\0';
        itoa(Char, stringa+2,10);
        debug_msg(stringa);
        #endif
        
        if (Char == SPACE) {
            sprite->y++;
            drawsprite(sprite);
            #ifdef DEBUG
            debug_msg("FALL RIGTH OK");
            #endif
        }   else {
            #ifdef DEBUG
            debug_msg("CHECK FALL RIGTH EXIT");
            #endif
            return;
        }
    }

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

void movesprite_left(struct _SPRITE *sprite){
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

    // check fall routine
    check_fall_left(sprite);
    
    // check wall routine
    Char = read_char(sprite->Read_Sprite_Pos_Char_X, sprite->Read_Sprite_Pos_Char_Y);
    if (Char == WALL) {
        sprite->wall = TRUE;
        #ifdef DEBUG
        debug_msg(" MOVE LEFT - WALL 1");
        #endif        
    } else if (Char == LADDER){
        sprite->ladder = TRUE;
        #ifdef DEBUG
        debug_msg(" MOVE LEFT - LADDER 1");
        #endif  
    } else if (Char == KEYS){
        sprite->key = TRUE;
        #ifdef DEBUG
        debug_msg(" MOVE LEFT - KEY 1");
        #endif  
    } else if (Char == DOORS){
        sprite->door = TRUE;
        #ifdef DEBUG
        debug_msg(" MOVE LEFT - DOOR 1");
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

    Char = read_char(sprite->Read_Sprite_Pos_Char_X+1, sprite->Read_Sprite_Pos_Char_Y+1);
    if (Char == WALL) {
        sprite->wall = TRUE;
        #ifdef DEBUG
        debug_msg(" MOVE LEFT - WALL 2");
        #endif        
    } else if (Char == LADDER){
        sprite->ladder = TRUE;
        #ifdef DEBUG
        debug_msg(" MOVE LEFT - LADDER 2");
        #endif  
    } else if (Char == KEYS){
        sprite->key = TRUE;
        #ifdef DEBUG
        debug_msg(" MOVE LEFT - KEY 2");
        #endif  
    } else if (Char == DOORS){
        sprite->door = TRUE;
        #ifdef DEBUG
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
        #ifdef DEBUG
        debug_msg(" MOVE LEFT - WALL 3");
        #endif        
    } else if (Char == LADDER){
        sprite->ladder = TRUE;
        #ifdef DEBUG
        debug_msg(" MOVE LEFT - LADDER 3");
        #endif  
    } else if (Char == KEYS){
        sprite->key = TRUE;
        #ifdef DEBUG
        debug_msg(" MOVE LEFT - KEY 3");
        #endif  
    } else if (Char == DOORS){
        sprite->door = TRUE;
        #ifdef DEBUG
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

}

void movesprite_rigth(struct _SPRITE *sprite){
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
    sprite->Read_Sprite_Pos_Char_X = (sprite->x/8)-2;
    //sprite->Read_Sprite_Pos_Char_Y = ((sprite->y/8)-6) > 0  ? ((sprite->y/8)-6) : 0 ;
    sprite->Read_Sprite_Pos_Char_Y = ((sprite->y/8)-4) > 0  ? ((sprite->y/8)-4) : 0 ;
    sprite->direction = RIGTH;


    #ifdef DEBUG
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

    // check fall routine
    check_fall_rigth(sprite);
    
    // check collision routine
    Char = read_char(sprite->Read_Sprite_Pos_Char_X, sprite->Read_Sprite_Pos_Char_Y);
    if (Char == WALL) {
        sprite->wall = TRUE;
        #ifdef DEBUG
        debug_msg(" MOVE RIGTH - WALL 1");
        #endif        
    } else if (Char == LADDER){
        sprite->ladder = TRUE;
        #ifdef DEBUG
        debug_msg(" MOVE RIGTH - LADDER 1");
        #endif  
    } else if (Char == KEYS){
        sprite->key = TRUE;
        #ifdef DEBUG
        debug_msg(" MOVE RIGTH - KEY 1");
        #endif  
    } else if (Char == DOORS){
        sprite->door = TRUE;
        #ifdef DEBUG
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

    Char = read_char(sprite->Read_Sprite_Pos_Char_X+1, sprite->Read_Sprite_Pos_Char_Y+1);
    if (Char == WALL) {
        sprite->wall = TRUE;
        #ifdef DEBUG
        debug_msg(" MOVE RIGTH - WALL 2");
        #endif        
    } else if (Char == LADDER){
        sprite->ladder = TRUE;
        #ifdef DEBUG
        debug_msg(" MOVE RIGTH - LADDER 2");
        #endif  
    } else if (Char == KEYS){
        sprite->key = TRUE;
        #ifdef DEBUG
        debug_msg(" MOVE RIGTH - KEY 2");
        #endif  
    } else if (Char == DOORS){
        sprite->door = TRUE;
        #ifdef DEBUG
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
        #ifdef DEBUG
        debug_msg(" MOVE RIGTH - WALL 3");
        #endif        
    } else if (Char == LADDER){
        sprite->ladder = TRUE;
        #ifdef DEBUG
        debug_msg(" MOVE RIGTH - LADDER 3");
        #endif  
    } else if (Char == KEYS){
        sprite->key = TRUE;
        #ifdef DEBUG
        debug_msg(" MOVE RIGTH - KEY 3");
        #endif  
    } else if (Char == DOORS){
        sprite->door = TRUE;
        #ifdef DEBUG
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

    // todo: check detect collect routine

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
}

void movesprite_up(struct _SPRITE *sprite){
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

    /*
    if (sprite->direction == LEFT) {

        sprite->Read_Sprite_Pos_Char_X = ((sprite->x+4)/8)-3;
        sprite->Read_Sprite_Pos_Char_Y = (((sprite->y+2)/8)-6) > 0  ? (((sprite->y+2)/8)-6) : 0 ;        
    }
    else {
 
        sprite->Read_Sprite_Pos_Char_X = ((sprite->x+4)/8)-3;
        sprite->Read_Sprite_Pos_Char_Y = (((sprite->y+2)/8)-6) > 0  ? (((sprite->y+2)/8)-6) : 0 ;
            
    }
 

    #ifdef DEBUG
    debug_msg("MOVE SPRITE UP");
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

    // sprite change shape

    // check fall routine
    
    // check collision routine

    Char = read_char(sprite->Read_Sprite_Pos_Char_X+1, sprite->Read_Sprite_Pos_Char_Y+2);

    if (Char == WALL) {
        sprite->wall = TRUE;
        sprite->collision = TRUE;
    } else if (Char == LADDER){
        sprite->y--;
        sprite->ladder = TRUE;
        sprite->collision = TRUE;
    }    
    */

}

void movesprite_down(struct _SPRITE *sprite){
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

    /*
    sprite->collision = FALSE;

    if (sprite->direction == LEFT) {

        sprite->Read_Sprite_Pos_Char_X = ((sprite->x+4)/8)-3;
        sprite->Read_Sprite_Pos_Char_Y = (((sprite->y+3)/8)-6) > 0  ? (((sprite->y+3)/8)-6) : 0 ;        
    }
    else {
 
        sprite->Read_Sprite_Pos_Char_X = ((sprite->x+4)/8)-3;
        sprite->Read_Sprite_Pos_Char_Y = (((sprite->y+3)/8)-6) > 0  ? (((sprite->y+3)/8)-6) : 0 ;
            
    }
 

    #ifdef DEBUG
    debug_msg("MOVE SPRITE UP");
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

    // sprite change shape

    // check fall routine
    
    // check collision routine

    Char = read_char(sprite->Read_Sprite_Pos_Char_X+1, sprite->Read_Sprite_Pos_Char_Y+2);
    if (Char == WALL) {
        sprite->wall = TRUE;
        sprite->collision = TRUE;
    } else if (Char == LADDER){
        sprite->y++;
        sprite->ladder = TRUE;
        sprite->collision = TRUE;
    }
    */
   
}

void jumpsprite(struct _SPRITE *sprite){
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

void jumpsprite_rigth(struct _SPRITE *sprite){
    int count = 0;

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

        //todo: check if the sprite reachs the floor
    }
}      