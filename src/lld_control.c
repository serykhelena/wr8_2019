#include <tests.h>
#include <lld_control.h>
#include <math.h>

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
  //For Speed (width: 1550 - 1680)
  float Speed_k_max;
  float Speed_b_max;

  //For Speed (width: 1350 - 1450)
  float Speed_k_min;
  float Speed_b_min;

  //For Steer
  float Steer_k;
  float Steer_b;


/**
 * @brief   Initialize periphery connected to driver control
 */
void lldControlInit( void )
{
  /*** PWM pins configuration ***/
  palSetLineMode( PWM1_CH0,  PAL_MODE_ALTERNATE(1) );
  palSetLineMode( PWM1_CH1,  PAL_MODE_ALTERNATE(1) );
  pwmStart( PWMdriver, &pwm1conf );

  //For Speed (width: 1550 - 1680)
  Speed_k_max = (SPEED_WIDTH_FORW_MAX - SPEED_WIDTH_FORW_MIN)/(SPEED_O + abs(SPEED_MIN));
  Speed_b_max = SPEED_WIDTH_FORW_MIN + (SPEED_O * Speed_k_max);
  //For Speed (width: 1350 - 1450)
  Speed_k_min = (SPEED_WIDTH_BACKW_MAX - SPEED_WIDTH_BACKW_MIN)/(SPEED_MAX);
  Speed_b_min = SPEED_WIDTH_BACKW_MAX + (SPEED_O * Speed_k_min);

  //For Steer
  Steer_k = (STEER_WIDTH_MAX - STEER_WIDTH_MIN)/(STEER_MAX + STEER_MAX);
  Steer_b = STEER_WIDTH_MAX - (STEER_MAX * Steer_k);
}

/**
 * @brief   Set power for driving motor
 * @param [in]  a   Motor power value [-100 100]
 * @note    power (0, 100]  -> forward
 * @note    power [-100, 0} -> backward
 */
void lldControlSetDrivePower(controlValue_t inputPrc)
{
  inputPrc = CLIP_VALUE( inputPrc, SPEED_MIN, SPEED_MAX );
  if (inputPrc > 0)
  {
    int32_t  speedDuty = inputPrc * Speed_k_max + Speed_b_max;
    pwmEnableChannel( PWMdriver, SPEED_PWMch, speedDuty );

//    chprintf((BaseSequentialStream *)&SD7, "\t speedDuty(%d)\n\r ", speedDuty);
  }
  if (inputPrc < 0)
  {
    int32_t speedDuty = inputPrc * Speed_k_min + Speed_b_min;
    pwmEnableChannel( PWMdriver, SPEED_PWMch, speedDuty );

//    chprintf( (BaseSequentialStream *)&SD7, "\t speedDuty(%d)\n\r ", speedDuty);
  }
  if (inputPrc == 0)
    {
      int32_t  speedDuty = 1500;
      pwmEnableChannel( PWMdriver, SPEED_PWMch, speedDuty );
//    chprintf( (BaseSequentialStream *)&SD3, "\t speedDuty(%d)\n\r ", speedDuty);
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
  inputPrc = CLIP_VALUE( inputPrc, STEER_MIN, STEER_MAX );
  int32_t steerDuty = inputPrc * Steer_k + Steer_b;
  pwmEnableChannel( PWMdriver, STEER_PWMch, steerDuty );

//  chprintf( (BaseSequentialStream *)&SD7, "\t steerDuty(%d)\n\r ", steerDuty);
}

//void PrintKBsteer(void)
//{
//  int K_st = Steer_k * 100;
//  int B_st = Steer_b * 100;
//  chprintf( (BaseSequentialStream *)&SD7, "\t Steer_k(%d)\n\r ", K_st);
//  chprintf( (BaseSequentialStream *)&SD7, "\t Steer_b(%d)\n\r ", B_st);
//}
//
//void PrintKBspeed(void)
//{
//  int K_sp_max = Speed_k_max * 100;
//  int B_sp_max= Speed_b_max * 100;
//  int K_sp_min = Speed_k_min * 100;
//  int B_sp_min= Speed_b_min * 100;
//
//  chprintf( (BaseSequentialStream *)&SD7, "\t Speed_k_max(%d)\n\r ", K_sp_max);
//  chprintf( (BaseSequentialStream *)&SD7, "\t Speed_b_max(%d)\n\r ", B_sp_max);
//  chprintf( (BaseSequentialStream *)&SD7, "\t Speed_k_min(%d)\n\r ", K_sp_min);
//  chprintf( (BaseSequentialStream *)&SD7, "\t Speed_b_min(%d)\n\r ", B_sp_min);
//}
