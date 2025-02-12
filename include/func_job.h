
#ifndef __FUNC_JOB_H
#define __FUNC_JOB_H

#include <stdint.h>
#include <stddef.h>

#ifdef __clang__
__attribute__((leaf))
#endif
extern void wait_very_briefly(uint8_t);   

#endif