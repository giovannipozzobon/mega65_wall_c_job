
#ifndef __MEGA65_KERNFILEIO_H
#define __MEGA65_KERNFILEIO_H

#include <stdint.h>
#include <stddef.h>

struct map_t{
  uint8_t a; //zp
  uint8_t x; //zp+1
  uint8_t y; //zp+2
  uint8_t z; //zp+3
};

struct m65_dirent {
  uint32_t d_ino;    // File number of entry
  uint16_t d_off;    // Offset
  uint32_t d_reclen; // Length of this record
  uint16_t d_type;   // File type
  char d_name[256];  // Filename string of entry
};

extern struct map_t __attribute__((zpage)) map_regs;

extern struct m65_dirent __attribute__((zpage)) *m65_dirent_exchange;


#ifdef __clang__
__attribute__((leaf))
#endif
void fill(void);

#ifdef __clang__
__attribute__((leaf))
#endif
void setbank(void);

#ifdef __clang__
__attribute__((leaf))
#endif
void setbank_attic(void);

#ifdef __clang__
__attribute__((leaf))
#endif
void setbank_map_regs(struct map_t);


/**
 * @brief Closes all open files
 */
#ifdef __clang__
__attribute__((leaf))
#endif
void setfsload(void);

#ifdef __clang__
__attribute__((leaf))
#endif
void setfssave(void);

#ifdef __clang__
__attribute__((leaf))
#endif
void setname(void);


#ifdef __clang__
__attribute__((leaf))
#endif
char savefilewithpars(struct map_t);



#ifdef __clang__
__attribute__((leaf))
#endif
char loadfilewithpars(struct map_t);

#ifdef __clang__
__attribute__((leaf))
#endif
void setname2(char *);

#ifdef __clang__
__attribute__((leaf))
#endif
void setname1(char *, uint8_t);
  
#ifdef __clang__
__attribute__((leaf))
#endif
void hyppo_opendir(void);

#ifdef __clang__
__attribute__((leaf))
#endif
void hyppo_closedir();

#ifdef __clang__
__attribute__((leaf))
#endif
void hyppo_readdir();

#endif //__MEGA65_KERNFILEIO_H