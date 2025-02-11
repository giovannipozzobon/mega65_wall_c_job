#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <file_fun.h>
#include <global.h>
#include <mega65/debug_calypis.h>
#include <chips.h>

#define DEBUG

#pragma require __preserve_zp

struct map_t __attribute__((zpage)) map_regs;
struct m65_dirent __attribute__((zpage)) *m65_dirent_exchange;

unsigned char __far *attic_memory;
unsigned char bank_file;

void loadresource(void);
void setchar(unsigned char __far  *charset_begin);
void setscreen(unsigned char __far  *screen_begin);
void setsprite(void);
void set_480(void);
void title_setup(void);

void main(void) {


    loadresource();

    initial_setup();

    setchar(attic_memory_charset);
    setscreen(attic_memory_screen);
    setsprite();

    title_setup();

    return;

}

void loadresource(void) {
    
    //load the charset file
    bank_file = ATTIC_MEM;
    attic_memory = attic_memory_charset;
    loadfile(charfile);

    //load the screen file
    attic_memory = attic_memory_screen;
    loadfile(screenfile);

    //load the sprite file
    attic_memory = attic_memory_sprite;
    loadfile(spritefile);

    return;
}

void setchar(unsigned char __far  *charset_begin){
    char stringa [10];
    unsigned char __far *charset;
    charset = charset_begin;
    //charset =  (attic_memory_charset);
  
    debug_msg("CHARSET SET");
  
    int num_char_definitions = (LEN_CHARSET);
    itoa(num_char_definitions, stringa+1,10);
    stringa[0]='C';  
    stringa[6]='\0';
    debug_msg(stringa);
  
  
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
    int num_char_screen = (LEN_SCREEN)*2;
    //int num_char_screen = 1000;
    itoa(num_char_screen, stringa+1,10);
    stringa[0]='S';  
    stringa[6]='\0';
    debug_msg(stringa);
  
    itoa(((CHARSET_MEMORY / 64) >> 8),stringa+1,10);
    stringa[0]='T';  
    stringa[6]='\0';
    debug_msg(stringa);
  
    
    for (int c = 0; c < num_char_screen; c=c+2) {
        FAR_U8_PTR(SCREEN_MAP) [c] = screen[c];
        FAR_U8_PTR(SCREEN_MAP) [c+1] = ((CHARSET_MEMORY / 64) >> 8);
        //debug_char(screen[c]);
    }
};

void setsprite(void){

};


void title_setup() {

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
	//VIC2.DEN = 1;
		
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