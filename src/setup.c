#include <calypsi/intrinsics6502.h>
#include "chips.h"
#include "global.h"
#include "setup.h"
#include <sprite.h>

__attribute__((aligned(64))) unsigned int SPRITE_SPACE[360];

extern struct _SPRITE sprite[8];

void initial_setup(void){
	__disable_interrupts();

	// set the speed to 40MHz
	CPU.PORTDDR = 65;

	// bank out everything except for I/O
	CPU.PORTDDR = 0b00000111;
	CPU.PORT    = 0b00000101;
	
	// disable CIA interrupts
	/*CIA1.ICR = 0b01111111;
	CIA2.ICR = 0b01111111;

	CIA1.ICR;
	CIA2.ICR;
	*/

	// map I/O (Mega65 memory mapping)
	VIC3.ROM8  = 0;
	VIC3.ROMA  = 0;
	VIC3.ROMC  = 0;
	VIC3.CROM9 = 0;
	VIC3.ROME  = 0;
	
	__asm(" lda #0x00\n"
		  " tax\n"
		  " tay\n"
		  " taz\n"
		  " map\n"
		  " nop");
	
	
	// Enable the VIC4
	VIC3.KEY = 0x47;
	VIC3.KEY = 0x53;

	// disable RAM protection in banks 2 and 3
	poke(0xD640, 0x70);		// $d640 = HTRAP00
	__no_operation();		// clv would be better

	// blank the screen while we do our initial loading
	VIC2.DEN = 0;

	// disable raster interrupts
	VIC4.FNRST    = 0;
	VIC4.FNRSTCMP = 0;
	
	// enable 640 horizontal width
	VIC3.H640 = 1;
	
	// disable hot registers
	VIC4.HOTREG = 0;


}


void screen_setup() {

	// enable 640 horizontal width
	//VIC3.H640 = 1;
	VIC3.H640 = 0;
	
	// disable hot registers
	//VIC4.HOTREG = 0;
	VIC4.HOTREG = 0;
	
	// use wide character lookup (i.e. character data anywhere in memory)
	VIC4.CHR16 = 1;
	//VIC4.CHR16 = 0;
	
	// set multicolor mode - allows use of both color palettes
	// probably not necessary to turn this on, but it also doesn't hurt.
	VIC2.MCM = 1;
	

	// set 480 vertical resolution
	set_480();


	VIC4.LINESTEP    = TITLE_LINE_LENGTH;
	VIC4.CHRCOUNTLSB = TITLE_LINE_LENGTH/2;
	VIC4.CHRCOUNTMSB = (TITLE_LINE_LENGTH/2) >> 8;
	VIC4.DISPROWS    = TITLE_LINE_COUNT;

	// use NCM and FCM for all characters
	VIC4.FCLRLO = 0;	// lower block, i.e. 0-255
	VIC4.FCLRHI = 1;	// everything above 255

	// set tile map location
	VIC4.SCRNPTR    = SCREEN_MAP & 0xffff;
	VIC4.SCRNPTRBNK = (SCREEN_MAP & 0xf0000) >> 16;
	VIC4.SCRNPTRMB  = 0x0;

	// load the attribute map into $ff80000
	
    //run_dma_job((__far char *)&load_title_attrmap);

	// set the border and screen colors to red, to match the picture
	//VIC2.BORDERCOL = 28;
	//VIC2.SCREENCOL = 28;
	VIC2.BORDERCOL = 0;
	VIC2.SCREENCOL = 0;

	// unblank the screen
	VIC2.DEN = 1;
		
	// initalize the music
	//musicInit();
}

/// This procedure sets the top and bottom border positions to allow 480 pixels
/// to be displayed.
void set_480() {

	// set 480 vertical resolution - this is video standard dependent.
	if (VIC4.PALNTSC) {
		VIC4.TBDRPOSLSB  = 0x01;	// defaults to $37/55
		VIC4.TBDRPOSMSB  = 0;	    // defaults to $00/0
		VIC4.BBDRPOSLSB  = 0xe1;	// defaults to $c7/199
		VIC4.BBDRPOSMSB  = 0x1;	    // defaults to $01/1		(i.e. 455)
		
		VIC4.TEXTYPOSLSB = 0x01;
		VIC4.TEXTYPOSMSB = 0;
	} else {
		VIC4.TBDRPOSLSB  = 0x40;	// defaults to $68/104
		VIC4.TBDRPOSMSB  = 0;	    // defaults to $00/0
		VIC4.BBDRPOSLSB  = 0x20;	// defaults to $f8/248
		VIC4.BBDRPOSMSB  = 0x2;	    // defaults to $01/1		(i.e. 504)
		
		VIC4.TEXTYPOSLSB = 0x40;
		VIC4.TEXTYPOSMSB = 0;
	}
}

void sprite_setup(void){
  
	char *sprite_ptr;
	//sprite_ptr = (char *) (SPRITE_RAM);
	sprite_ptr = (char *) (SPRITE_SPACE);
	//debug_msg("SPRITE SET");
  
	// set data for 1st sprite
	SPRITE_PTRS[0] = (((int)sprite_ptr) >> 6);
  
	// set data for 1st sprite
	SPRITE_PTRS[1] = ((((int)sprite_ptr)+24*8) >> 6);
	
	#ifdef DEBUG
	char stringa [10];
	itoa((int)sprite_ptr, stringa+1,10);
	stringa[0]='S';  
	stringa[6]='\0';
	debug_msg(stringa);
  
	itoa(SPRITE_PTRS[0], stringa+1,10);
	stringa[0]='S';  
	stringa[6]='\0';
	debug_msg(stringa);
  
	itoa((unsigned int)SPRITE_PTRS, stringa+1,10);
	stringa[0]='S';  
	stringa[6]='\0';
	debug_msg(stringa);
	#endif
  
	// set location of 16 bit sprite pointers 
	VIC4.SPRPTRADR = (unsigned int)SPRITE_PTRS;
  
	// enable location of 16 bit sprite pointers bank 
	VIC4.SPRPTR16 = 0x01;
  
	// full colour sprite mode for sprites (SPR16EN)
	VIC4.SPR16EN = 0x03;
  
	// 16 pixel wide for sprites (SPRX64EN) 53335
	VIC4.SPRX64EN = 0x03;
  
	// 21 pixel high sprite heights
	VIC4.SPRHGHT = 21;
  
	// all sprites use height in $d056 sprhgten
	VIC4.SPRHGTEN = 0xFF;
  
	// sprite multicolor 1
	VIC2.SPRMC0 = 0x08;
  
	// sprite multicolor 1
	VIC2.SPRMC1 = 0x06;
  
	// sprite 0 position X
	VIC2.S0X = 0x20;
  
	// sprite 0 position Y
	VIC2.S0Y = 0x45;
  
	// sprite 1 position X
	VIC2.S1X = 0x80;
  
	// sprite 1 position Y
	VIC2.S1Y = 0x80;
  
	// Sprite enable (SE)
	VIC2.SE = 0x03;
  
	// Sprite 0 color 
	VIC2.SPR0COL = 0x00;
	
	// Sprite 1 color 
	VIC2.SPR1COL = 0x00;

  }
  

void setsprite(unsigned char __far  *sprite_begin){

    char *sprite;
    sprite = (char *) (sprite_begin);
  
  
    int len_sprite_definitions = LEN_SPRITE;

	#ifdef DEBUG
	debug_msg("SPRITE");
	char stringa [10];
	itoa(len_sprite_definitions, stringa+1,10);
    stringa[0]='C';  
    stringa[6]='\0';
    debug_msg(stringa);
  	#endif
  
    for (int c = 0; c < len_sprite_definitions; c++) {
        //FAR_U8_PTR(SPRITE_RAM) [c] = sprite_begin[c]; 
        FAR_U8_PTR(SPRITE_SPACE) [c] = sprite_begin[c]; 
        //debug_char(charset[c]);
    }

};


void setchar(unsigned char __far  *charset_begin){

    unsigned char __far *charset;
    charset = charset_begin;
    //charset =  (attic_memory_charset);

	int num_char_definitions = (LEN_CHARSET);
	
	#ifdef DEBUG
	char stringa [10];
    debug_msg("CHARSET SET");
  
    itoa(num_char_definitions, stringa+1,10);
    stringa[0]='C';  
    stringa[6]='\0';
    debug_msg(stringa);
	#endif
  
  
    for (int c = 0; c < num_char_definitions; c++) {
        FAR_U8_PTR(CHARSET_MEMORY) [c] = charset[c];
    }

};

void setscreen(unsigned char __far  *screen_begin){
    char stringa [10];
    unsigned char __far *screen;
    screen = screen_begin;
    debug_msg("SCREEN SET");
  
    // copy special characters
    int num_char_screen = (LEN_SCREEN);
    
	#ifdef DEBUG
	char stringa [10];

	itoa(num_char_screen, stringa+1,10);
	stringa[0]='S';  
	stringa[6]='\0';
	debug_msg(stringa);

	itoa(num_char_screen, stringa+1,10);
    stringa[0]='S';  
    stringa[6]='\0';
    debug_msg(stringa);
  
    itoa(((CHARSET_MEMORY / 64) >> 8),stringa+1,10);
    stringa[0]='T';  
    stringa[6]='\0';
    debug_msg(stringa);
	#endif
  
    
    for (int c = 0; c < num_char_screen; c=c+2) {
        FAR_U8_PTR(SCREEN_MAP) [c] = screen[c];
        FAR_U8_PTR(SCREEN_MAP) [c+1] = ((CHARSET_MEMORY / 64) >> 8);
        //debug_char(screen[c]);
    }
};

void colorset_init(void)
{

  //char *color;
  //color = (char *) &charcolor_begin;


  // copy special characters
  int num_color = 2000;

  #ifdef DEBUG
  char stringa [10];
  debug_msg("COLOR SET");
  itoa(num_color, stringa+1,10);
  stringa[0]='O';  
  stringa[6]='\0';
  debug_msg(stringa);
  #endif

  // il primo byte è zero e il secondo contiene un colore
  // non so se venga utilizzato perché sembra sempre uguale
  // da verificare come funziona perché non ho capito
  for (int c = 0; c < num_color; c=c+2) {
    FAR_U8_PTR(0xff80000) [c] = 0;
    FAR_U8_PTR(0xff80000) [c+1] = 10;
  }
}
