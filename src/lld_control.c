#include <tests.h>
#include <lld_control.h>

/**************************/
/*** CONFIGURATION ZONE ***/
/**************************/

static int    speed_min     = -100;
static int    speed_max     = 100;

float speed_dutyK = 0;
int speed_dutyB = 0;

static signed int    steer_min     = -100;
static signed int    steer_max     = 100;

float steer_dutyK = 4;
int steer_dutyB = 1560;

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
// пин G9 - канал №1 таймера №1
#define GPIOE_TIM1_CH1              9U
#define GPIOE_TIM1_CH2              11U
/***  PE9 - Speeding            ***/
#define PE9_ACTIVE      PWM_OUTPUT_ACTIVE_HIGH
#define PE9_DISABLE     PWM_OUTPUT_DISABLED
#define speedPWMch      0
/***  PE11 - Steering           ***/
#define PE11_ACTIVE     PWM_OUTPUT_ACTIVE_HIGH
#define PE11_DISABLE    PWM_OUTPUT_DISABLED
#define steerPWMch      1
/***  PE13, PE14 - not used     ***/
#define PE13_ACTIVE     PWM_OUTPUT_ACTIVE_HIGH
#define PE13_DISABLE    PWM_OUTPUT_DISABLED
#define PE14_ACTIVE     PWM_OUTPUT_ACTIVE_HIGH
#define PE14_DISABLE    PWM_OUTPUT_DISABLED

#define pwm1LineCh0     PAL_LINE( GPIOE, 9 )
#define pwm1LineCh1     PAL_LINE( GPIOE, 11 )

#define pwm1Freq        1000000
#define pwm1Period      20000       // 50 Hz

static  PWMDriver       *pwmDriver      = &PWMD1;

/*** Direction pins configuration          ***/
/*** F_12 for Driving Wheels Set Direction ***/
#define lineMotorDir        PAL_LINE( GPIOD, 3 )

/*** Configuration structures ***/

PWMConfig pwm1conf = { //PWM_period [s] = period / frequency
      .frequency = pwm1Freq,
      .period    = pwm1Period,
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
    palSetLineMode( pwm1LineCh0,  PAL_MODE_ALTERNATE(1) );
    palSetLineMode( pwm1LineCh1,  PAL_MODE_ALTERNATE(1) );
    pwmStart( pwmDriver, &pwm1conf );
}

/**
 * @brief   Set power for driving motor
 * @param [in]  a   Motor power value [-100 100]
 */

void lldControlSetMotor(controlValue_t inputPrc)
{
    inputPrc = CLIP_VALUE( inputPrc, speed_min, speed_max );
    if (inputPrc >= 0)
    {
      speed_dutyK = 0.5;
      speed_dutyB = 1400;
    }
    else
    {
      speed_dutyK = 0.8;
      speed_dutyB = 1320;
    }
    int speedDuty = inputPrc * speed_dutyK + speed_dutyB;


    pwmEnableChannel( pwmDriver, speedPWMch, speedDuty );

    //test duty motor
    //chprintf(((BaseSequentialStream *)serialDriver), "DUTY(%d)\r", speedDuty );
}

/**
 * @brief   TEST set power for driving motor
 * @param   a   Motor power value [+/- 10]
 */
//void lldControlSetMotor2(controlValue_t inputPrc)
//{
//    pwmEnableChannel( pwmDriver, speedPWMch, 1480+inputPrc );
//}

/*
 * @brief   Set power for steering motor (via ESC)
 * @param   a   Motor power value [-100 100]
 * @note    power (0, 100]  -> clockwise
 * @note    power [-100, 0} -> counterclockwise
 */
void lldControlSetSteer(controlValue_t inputPrc)
{
    inputPrc = CLIP_VALUE( inputPrc, steer_min, steer_max );
    int steerDuty = inputPrc * steer_dutyK + steer_dutyB;

    pwmEnableChannel( pwmDriver, steerPWMch, steerDuty );
}

/**
 * @brief   TEST Set power for steering motor (via ESC)
 * @param   a   Motor power value [+/- 10]
 */
//void lldControlSetSteer2(controlValue_t inputPrc)
//{
//    pwmEnableChannel( pwmDriver, steerPWMch, 1510+inputPrc );
//}
