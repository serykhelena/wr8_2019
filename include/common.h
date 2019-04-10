#ifndef INCLUDE_COMMON_H_
#define INCLUDE_COMMON_H_

#include <ch.h>
#include <hal.h>

#include <stdint.h>
#include <stdlib.h>

#define     PROGRAM_ROUTINE_MASTER                      0
#define     PROGRAM_ROUTINE_TEST_ENCODER                1
#define     PROGRAM_ROUTINE_TEST_LL_DRIVER              2
#define     PROGRAM_ROUTINE_TEST_RM_CONTROL             3


#define     MAIN_PROGRAM_ROUTINE                        PROGRAM_ROUTINE_TEST_LL_DRIVER


#define CLIP_VALUE(x, min, max) ((x) < (min) ? (min) :      \
                                 (x) > (max) ? (max) : (x))

void debug_stream_init( void );
void dbgprintf( const char* format, ... );

#endif /* INCLUDE_COMMON_H_ */
