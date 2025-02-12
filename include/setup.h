#ifndef __SETUP_H
#define __SETUP_H

__attribute__((aligned(16))) unsigned int SPRITE_PTRS[8];
__attribute__((aligned(64))) unsigned int SPRITE_SPACE[360];

void initial_setup(void);
void set_480();
void screen_setup();
void sprite_setup(void);
void setsprite(unsigned char __far  *sprite_begin);
void colorset_init(void);
void setscreen(unsigned char __far  *screen_begin);
void setchar(unsigned char __far  *charset_begin);


#endif