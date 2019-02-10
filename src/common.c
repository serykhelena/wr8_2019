#include <common.h>


/**************************/
/*** CONFIGURATION ZONE ***/
/**************************/

#define SPEEDmin           -100
#define SPEEDmax            100

#define SPEED_DUTY_K_max    4
#define SPEED_DUTY_B_max    1520

#define SPEED_DUTY_K_min    2.6
#define SPEED_DUTY_B_min    1420

#define STEERmin           -100
#define STEERmax            100

#define STEER_DUTY_K        4
#define STEER_DUTY_B        1560

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
