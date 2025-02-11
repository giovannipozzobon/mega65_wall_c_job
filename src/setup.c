#include <calypsi/intrinsics6502.h>
#include "chips.h"
#include "global.h"


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
	
	// load the minimal data required to show the titlescreen
	//load_loader();
		
	
	// disable raster interrupts
	VIC4.FNRST    = 0;
	VIC4.FNRSTCMP = 0;
	
	// enable 640 horizontal width
	VIC3.H640 = 1;
	
	// disable hot registers
	VIC4.HOTREG = 0;


}