#include <file_fun.h>
#include <debug_calypis.h>
#include <global.h>
#include <string.h>

//#define DEBUG

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
  
    #ifdef DEBUG
    debug_msg("SET MAP OK");
    #endif

    //setbank_attic();
    setbank_map_regs(map_regs);
    
    #ifdef DEBUG
    debug_msg("SET BANK OK");
    #endif

    //setbank();
    setfsload();
    //setname();
    //sname(filename1);
  
  
    #ifdef DEBUG
      debug_msg(file);
    #endif
    
    #ifdef DEBUG
    char stringa[10];
    stringa[0]='S';  
    stringa[1]=':';  
    stringa[6]='\0';
    itoa(strlen(file), stringa+2,10);
    debug_msg(stringa);
    #endif
  
    setname1(file, (uint8_t) strlen(file));
    //setname1(filename1, (uint8_t) strlen(filename1));  

    #ifdef DEBUG
    debug_msg("SET NAME OK");
    #endif
    
     // Set params for load
    map_regs.a = 0x40;    
    map_regs.y = ((int)attic_memory &0xFF00) >> 8; //bank of start adress
    map_regs.x = 0x00;

    #ifdef DEBUG
    debug_msg("SET PARAMS OK");
    #endif
  
    if ((loadfilewithpars(map_regs)) == 0x00) {
        #ifdef DEBUG
        debug_msg(" LOAD OK"); 
        #endif
      }
      else {
        #ifdef DEBUG
        debug_msg(" LOAD ERROR");
        #endif
      };
    
      free (file);
  }
  