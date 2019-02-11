#include <tests.h>
#include <lld_control.h>
<<<<<<< HEAD
#include <math.h>
=======
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
>>>>>>> ed87bdeacda3aa89c6954fbf65c6288514ad4730

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

/*** Coefficients declaration K & B ***/
  //For Speed (width: 1500 - 1600)
  controlValue_t Speed_k_max;
  controlValue_t Speed_b_max;
  //For Speed (width: 1240 - 1400)
  controlValue_t Speed_k_min;
  controlValue_t Speed_b_min;
  //For Steer
  controlValue_t Steer_k;
  controlValue_t Steer_b;


/**
 * @brief   Initialize periphery connected to driver control
 */
void lldControlInit( void )
{
  /*** PWM pins configuration ***/
  palSetLineMode( PWM1_CH0,  PAL_MODE_ALTERNATE(1) );
  palSetLineMode( PWM1_CH1,  PAL_MODE_ALTERNATE(1) );
  pwmStart( PWMdriver, &pwm1conf );

  //For Speed (width: 1500 - 1600)
  Speed_k_max = (SPEED_WIDTH_FORW_MAX - SPEED_WIDTH_FORW_MIN)/(SPEED_O + abs(SPEED_MIN));
  Speed_b_max = SPEED_WIDTH_FORW_MAX + (SPEED_O * Speed_k_max);
  //For Speed (width: 1240 - 1400)
  Speed_k_min = (SPEED_WIDTH_BACKW_MAX - SPEED_WIDTH_BACKW_MIN)/(SPEED_O + SPEED_MAX);
  Speed_b_min = SPEED_WIDTH_BACKW_MIN + (SPEED_O * Speed_k_min);
  //For Steer
  Steer_k = (STEER_WIDTH_MAX - STEER_WIDTH_MINimum)/(abs(STEER_MIN) + STEER_MAX);
  Steer_b = STEER_WIDTH_MINimum + (abs(STEER_MIN) * Steer_k);
}

/**
 * @brief   Set power for driving motor
 * @param [in]  a   Motor power value [-100 100]
 * @note    power (0, 100]  -> forward
 * @note    power [-100, 0} -> backward
 */
void lldControlSetDrivePower(controlValue_t inputPrc)
{
  inputPrc = CLIP_VALUE( inputPrc, SPEEDmin, SPEEDmax );
  if (inputPrc >= 0)
  {
<<<<<<< HEAD
    int32_t speedDuty = inputPrc * Speed_k_max + Speed_b_max;
=======
    int32_t   speedDuty = inputPrc * SPEED_DUTY_K_max + SPEED_DUTY_B_max;
>>>>>>> ed87bdeacda3aa89c6954fbf65c6288514ad4730
    pwmEnableChannel( PWMdriver, SPEED_PWMch, speedDuty );
  }
  else
  {
<<<<<<< HEAD
    int32_t speedDuty = inputPrc * Speed_k_min + Speed_b_min;
=======
    int32_t   speedDuty = inputPrc * SPEED_DUTY_K_min + SPEED_DUTY_B_min;
>>>>>>> ed87bdeacda3aa89c6954fbf65c6288514ad4730
    pwmEnableChannel( PWMdriver, SPEED_PWMch, speedDuty );
  }
}

/*
 * @brief   Set power for steering motor
 * @param   a   Motor power value [-100 100]
 * @note    power (0, 100]  -> clockwise
 * @note    power [-100, 0} -> counterclockwise
 */
void lldControlSetSteerPower(controlValue_t inputPrc)
{
<<<<<<< HEAD
  inputPrc = CLIP_VALUE( inputPrc, STEER_MIN, STEER_MAX );
  int32_t steerDuty = inputPrc * Steer_k + Steer_b;
=======
  inputPrc = CLIP_VALUE( inputPrc, STEERmin, STEERmax );
  int32_t steerDuty = inputPrc * STEER_DUTY_K + STEER_DUTY_B;

>>>>>>> ed87bdeacda3aa89c6954fbf65c6288514ad4730
  pwmEnableChannel( PWMdriver, STEER_PWMch, steerDuty );
}

