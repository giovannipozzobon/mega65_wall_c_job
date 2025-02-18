#include <file_fun.h>
#include <debug_calypis.h>
#include <global.h>
#include <string.h>

#define DEBUG

unsigned char * makefilename(char * filename){
    unsigned char * namefile;
    namefile = malloc(strlen("@:") + strlen(filename) + strlen(",SEQ") + 1);
    strcpy(namefile,"@:");
    strcat(namefile, filename);
    strcat(namefile, ",SEQ");
  
    #ifdef DEBUG
      debug_msg(namefile);
    #endif

    return namefile;
  }

void loadfile(char * filename){
    unsigned char * file; 
  

    file = makefilename(filename);

    // set the map_regs for memory attic
    map_regs.a = bank_file;
    map_regs.y = ((long)attic_memory) >> 16;
    map_regs.x = 0x00;
  
  
    //setbank_attic();
    setbank_map_regs(map_regs);
  
    //setbank();
    setfsload();
    //setname();
    //sname(filename1);
  
  
    #ifdef DEBUG
      debug_msg(file);
    #endif
    
    setname1(file, (uint8_t) strlen(file));
    //setname1(filename1, (uint8_t) strlen(filename1));  
    
    
     // Set params for load
    map_regs.a = 0x40;    
    map_regs.y = ((int)attic_memory &0xFF00) >> 8; //bank of start adress
    map_regs.x = 0x00;
  
  
    if ((loadfilewithpars(map_regs)) == 0x00) {
        debug_msg(" LOAD OK"); 
      }
      else {
        debug_msg(" LOAD ERROR");
      };
    
      free (file);
  }
  