
#ifndef __GLOBAL_H
#define __GLOBAL_H

#define attic_mem 0x88

extern unsigned char __far  *attic_memory_graph = 0x8064500;
char graphfile [] = {72, 69, 76, 80, 74, 69, 68, 73, 84, 44, 83, 0x00};

extern char *filetoloadsave;
extern unsigned char __far *attic_memory;
extern  unsigned char bank_file;



#endif //__GLOBAL_H