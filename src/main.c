#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <file_fun.h>
#include <global.h>
#include <mega65/debug_calypis.h>

#define DEBUG

struct map_t __attribute__((zpage)) map_regs;
struct m65_dirent __attribute__((zpage)) *m65_dirent_exchange;

char *filetoloadsave;
unsigned char __far *attic_memory;
unsigned char bank_file;

void main(void) {


    //load the graphic file
    filetoloadsave = graphfile;
    bank_file = attic_mem;
    attic_memory = attic_memory_graph;
    filetoloadsave = graphfile;
    loadfile('h');


}