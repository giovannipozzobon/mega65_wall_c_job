
#ifndef __GLOBAL_H
#define __GLOBAL_H

#define FAR_U8_PTR(X) ((uint8_t __far *)(X))
#define FAR_I8_PTR(X) ((int8_t __far *)(X))
#define FAR_U16_PTR(X) ((uint16_t __far *)(X))
#define FAR_I16_PTR(X) ((int16_t __far *)(X))

//generic constants
#define TRUE 1
#define FALSE 0

// directions of sprites
#define LEFT 1
#define RIGTH 2
#define DOWN 3
#define UP 4

// types of chars of the screen
#define WALL 1
#define SPACE 0x20
#define LADDER 4
#define KEYS 2
#define DOORS 3

#define ATTIC_MEM 0x88

//screen costants
#define LEN_CHARSET 16384
#define LEN_SCREEN  2000
#define LEN_SPRITE  360

#define TITLE_LINE_LENGTH     80
#define TITLE_LINE_COUNT      24

// sprite delay for movement
#define DELAY_JOY 2

/// memoria riservata per i caratteri jobond 20/11/2024
#define CHARSET_MEMORY 0x44000
/// memoria riservata per gli sprite jobond 20/11/2024
//#define SPRITE_RAM  0x5a000   //  Sprite RAM
/// \brief	A pointer to where the title screen's tile map is stored
#define SCREEN_MAP        0x0050000//0x0059400
/// \brief	A pointer to where the title screen's attribute map is stored
#define ATTR_MAP        0x803ba00

/// \brief	Set a memory location to a value
///
/// \param	a	The address to set
/// \param	t	The value to set the address to
#define poke(a, t) *((volatile char *)a) = t

/// \brief	Set a memory location in upper memory to a value
///
/// \param	a	The address to set
/// \param	t	The value to set the address to
#define farpoke(a, t) *((volatile __far char *)a) = t

/// \brief	Read a memory location
///
/// \param	a	The address to read
/// \returns	The value at the address
#define peek(a) *((volatile char *)a)

/// \brief	Set a specific bit mask at a memory location
///
/// \param	a	The address to set
/// \param	t	The bitmask to set at the location
#define set_bit(a, t) *((char *)a) |= t

/// \brief	Clear a specific bit mask at a memory location
///
/// \param	a	The address to clear
/// \param	t	The bitmask to clear at the location
#define clear_bit(a, t) *((char *)a) &= ~t


unsigned char __far  *attic_memory_charset = 0x8014500;
unsigned char __far  *attic_memory_sprite = 0x8024500;
unsigned char __far  *attic_memory_screen = 0x8034500;
unsigned char __far  *attic_memory_attr = 0x8044500;
/*
unsigned char __far  *attic_memory5 = 0x8054500;
unsigned char __far  *attic_memory_help = 0x8064500;
unsigned char __far  *attic_memory_util = 0x8074500;
unsigned char __far  *attic_memory_dma = 0x8084500;
unsigned char __far  *attic_memory_buffer = 0x8100000;
*/

char charfile [] = "CHARS";
char screenfile [] = "SCREEN";
char spritefile [] = "SPRITE";

extern unsigned char __far *attic_memory;
extern  unsigned char bank_file;

#endif //__GLOBAL_H