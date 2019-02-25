#ifndef INCLUDE_COMMON_H_
#define INCLUDE_COMMON_H_

#include <ch.h>
#include <hal.h>

#include <stdint.h>
#include <stdlib.h>

#define     PROGRAM_ROUTINE_MASTER                      0
#define     PROGRAM_ROUTINE_TEST_ENCODER                1
#define     PROGRAM_ROUTINE_TEST_LL_DRIVER              2
#define     PROGRAM_ROUTINE_TEST_CHARGE_LEVEL           3

#define     MAIN_PROGRAM_ROUTINE                        PROGRAM_ROUTINE_TEST_LL_DRIVER


#define CLIP_VALUE(x, min, max) ((x) < (min) ? (min) :      \
                                 (x) > (max) ? (max) : (x))

/**************************/
/*** CONFIGURATION ZONE ***/
/**************************/

#define SPEED_MIN               (float)-100
#define SPEED_O                  (float)0
#define SPEED_MAX                (float)100

#define SPEED_WIDTH_BACKW_MIN    (float)1240
#define SPEED_WIDTH_BACKW_MAX    (float)1400

#define SPEED_WIDTH_FORW_MIN     (float)1500
#define SPEED_WIDTH_FORW_MAX     (float)1600

#define STEER_MIN               (float)-100
#define STEER_MAX                (float)100

#define STEER_WIDTH_MIN          (float)1200
#define STEER_WIDTH_MAX          (float)2080


// *        |  Clockwise  |  Center  | Counterclockwise
// * -------------------------------------------------
// * t, ms  |     1.16    |   1.56   |      1.92
// * -------------------------------------------------
// * width  |     1200    |   1620   |      2080
// * -------------------------------------------------
// *        |on the right |  Center  |   On the left
// *
// * PD_15 => Driving wheels (Channel 2)
// *
// *        | Backward  |     Center      | Forward
// *-------------------------------------------
// * t, ms  |    1.24   |    1.4 - 1.5    |   1.6
// * ------------------------------------------
// * width  |    1240   |   1400 - 1500   |   1600
// *

/******************************/
/*** CONFIGURATION ZONE END ***/
/******************************/



#endif /* INCLUDE_COMMON_H_ */
