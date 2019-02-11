#ifndef INCLUDE_COMMON_H_
#define INCLUDE_COMMON_H_

#include <ch.h>
#include <hal.h>

#include <stdint.h>
#include <stdlib.h>

#define     PROGRAM_ROUTINE_MASTER                      0
#define     PROGRAM_ROUTINE_TEST_ENCODER                1
#define     PROGRAM_ROUTINE_TEST_LL_DRIVER              2

#define     MAIN_PROGRAM_ROUTINE                        PROGRAM_ROUTINE_TEST_ENCODER


#define CLIP_VALUE(x, min, max) ((x) < (min) ? (min) :      \
                                 (x) > (max) ? (max) : (x))

<<<<<<< HEAD
/**************************/
/*** CONFIGURATION ZONE ***/
/**************************/

#define SPEED_MIN               -100
#define SPEED_O                  0
#define SPEED_MAX                100

#define SPEED_WIDTH_BACKW_MIN    1240
#define SPEED_WIDTH_BACKW_MAX    1400

#define SPEED_WIDTH_FORW_MIN     1500
#define SPEED_WIDTH_FORW_MAX     1600


#define STEER_MIN               -100
#define STEER_MAX                100

#define STEER_WIDTH_MINimum      1200
#define STEER_WIDTH_MAX          2000


// *        |  Clockwise  |  Center  | Counterclockwise
// * -------------------------------------------------
// * t, ms  |     1.16    |   1.56   |      1.92
// * -------------------------------------------------
// * width  |     1160    |   1560   |      1920
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
=======
>>>>>>> ed87bdeacda3aa89c6954fbf65c6288514ad4730


#endif /* INCLUDE_COMMON_H_ */
