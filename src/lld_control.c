#include <tests.h>
#include <lld_control.h>
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

//*        |  Clockwise  |  Center  | Counterclockwise
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

/*** Hardware configuration     ***/
/***  PWM configuration pins    ***/
/***  PE9 - Speeding            ***/
#define PE9_ACTIVE      PWM_OUTPUT_ACTIVE_HIGH
#define PE9_DISABLE     PWM_OUTPUT_DISABLED
#define SPEED_PWMch      0
/***  PE11 - Steering           ***/
#define PE11_ACTIVE     PWM_OUTPUT_ACTIVE_HIGH
#define PE11_DISABLE    PWM_OUTPUT_DISABLED
#define STEER_PWMch      1
/***  PE13, PE14 - not used     ***/
#define PE13_ACTIVE     PWM_OUTPUT_ACTIVE_HIGH
#define PE13_DISABLE    PWM_OUTPUT_DISABLED
#define PE14_ACTIVE     PWM_OUTPUT_ACTIVE_HIGH
#define PE14_DISABLE    PWM_OUTPUT_DISABLED

#define PWM1_CH0        PAL_LINE( GPIOE, 9 )
#define PWM1_CH1        PAL_LINE( GPIOE, 11 )

#define PWM1_FREQ       1000000
#define PWM1_PERIOD     20000       // 50 Hz

static  PWMDriver       *PWMdriver      = &PWMD1;

/*** Configuration structures ***/

PWMConfig pwm1conf = { //PWM_period [s] = period / frequency
      .frequency = PWM1_FREQ,
      .period    = PWM1_PERIOD,
      .callback  = NULL,
      .channels  = {
                    {.mode = PE9_ACTIVE,      .callback = NULL},
                    {.mode = PE11_ACTIVE,     .callback = NULL},
                    {.mode = PE13_DISABLE,    .callback = NULL},
                    {.mode = PE14_DISABLE,    .callback = NULL}
                    },
      .cr2        = 0,
      .dier       = 0
  };

/***********************************************************/

/**
 * @brief   Initialize periphery connected to driver control
 */
void lldControlInit( void )
{
    /*** PWM pins configuration ***/
    palSetLineMode( PWM1_CH0,  PAL_MODE_ALTERNATE(1) );
    palSetLineMode( PWM1_CH1,  PAL_MODE_ALTERNATE(1) );
    pwmStart( PWMdriver, &pwm1conf );
}

/**
 * @brief   Set power for driving motor
 * @param [in]  a   Motor power value [-100 100]
 */
void lldControlDrivingWheels(controlValue_t inputPrc)
{
    inputPrc = CLIP_VALUE( inputPrc, SPEEDmin, SPEEDmax );
    if (inputPrc >= 0)
    {
      int speedDuty = inputPrc * SPEED_DUTY_K_max + SPEED_DUTY_B_max;
      pwmEnableChannel( PWMdriver, SPEED_PWMch, speedDuty );
    }
    else
    {
      int speedDuty = inputPrc * SPEED_DUTY_K_min + SPEED_DUTY_B_min;
      pwmEnableChannel( PWMdriver, SPEED_PWMch, speedDuty );
    }
}

/*
 * @brief   Set power for steering motor (via ESC)
 * @param   a   Motor power value [-100 100]
 * @note    power (0, 100]  -> clockwise
 * @note    power [-100, 0} -> counterclockwise
 */
void lldControlSteeringWheels(controlValue_t inputPrc)
{
    inputPrc = CLIP_VALUE( inputPrc, STEERmin, STEERmax );
    int steerDuty = inputPrc * STEER_DUTY_K + STEER_DUTY_B;

    pwmEnableChannel( PWMdriver, STEER_PWMch, steerDuty );
}

