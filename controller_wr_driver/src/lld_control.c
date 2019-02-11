#include <tests.h>
//#include <common.h>
#include <lld_control.h>


#define pwm1Freq        1000000
#define pwm1Period      20000           // 50 Hz

/***  PWM configuration pins    ***/
/***  PE9 - Steering            ***/
#define PE9_ACTIVE      PWM_OUTPUT_ACTIVE_HIGH
#define PE9_DISABLE     PWM_OUTPUT_DISABLED
#define drivePWMch      0
/***  PE11 - Braking            ***/
#define PE11_ACTIVE     PWM_OUTPUT_ACTIVE_HIGH
#define PE11_DISABLE    PWM_OUTPUT_DISABLED
#define steerPWMch      1
/***  PE13, PE14 - not used     ***/
#define PE13_ACTIVE     PWM_OUTPUT_ACTIVE_HIGH
#define PE13_DISABLE    PWM_OUTPUT_DISABLED
#define PE14_ACTIVE     PWM_OUTPUT_ACTIVE_HIGH
#define PE14_DISABLE    PWM_OUTPUT_DISABLED

static  PWMDriver       *pwmDriver      = &PWMD1;

#define pwm1LineCh0     PAL_LINE( GPIOE, 9 )
#define pwm1LineCh1     PAL_LINE( GPIOE, 11 )


//#define SPEED_MAX_DC    1920
//#define SPEED_MIN_DC    1160
//#define SPEED_NULL_DC   1540
//
//#define STEER_MAX       2040
//#define STEER_MIN       1200
//#define STEER_NULL      1620
//
//#define CONTROL_MAX     100
//#define CONTROL_MIN     (-100)


/*** Koefficient calculation ***/
/*
 * k = (SPEED_MAX_DC - SPEED_MIN_DC) / (CONTOL_MAX - CONTROL_MIN)
 * b = SPEED_MIN_DC - k * CONTROL_MIN
 */

//#define SPEED_FORWARD_K         3.8
//#define SPEED_FORWARD_B         1520
//
//#define SPEED_BACKWARD_K        2.6
//#define SPEED_BACKWARD_B        1420

//#define STEER_K         4.2
//#define STEER_B         1620

/*** Configuration structures ***/

PWMConfig pwm1conf = {
    .frequency = pwm1Freq,
    .period    = pwm1Period, /* 1/1000 s = 10 ms => 100 Hz
                             * PWM period = period/frequency [s] */
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

static bool         isInitialized       = false;

rawPwmValue_t       drDuty              = 0;

float               lld_speed_forward_k = 0;
float               lld_speed_forward_b = 0;

float               lld_speed_back_k    = 0;
float               lld_speed_back_b    = 0;

float               lld_steer_k         = 0;
float               lld_steer_b         = 0;


/**
 * @brief   Initialize periphery connected to driver control
 * @note    Stable for repeated calls
 */
void lldControlInit( void )
{
    if ( isInitialized )
        return;

    /*** PWM pins configuration ***/
    palSetLineMode( pwm1LineCh0,  PAL_MODE_ALTERNATE(1) );
    palSetLineMode( pwm1LineCh1,  PAL_MODE_ALTERNATE(1) );

    pwmStart( pwmDriver, &pwm1conf );


    /*** Calibration coefficients calculation ***/
    lld_speed_forward_k = (float)( SPEED_MAX - SPEED_NULL_FORWARD )/( CONTROL_MAX - CONTROL_NULL );
    lld_speed_forward_b = ( SPEED_NULL_FORWARD - lld_speed_forward_k * CONTROL_NULL );

    lld_speed_back_k = (float)( SPEED_NULL_BACK - SPEED_MIN )/( CONTROL_NULL - CONTROL_MIN );
    lld_speed_back_b = ( SPEED_MIN - lld_speed_back_k * CONTROL_MIN );

    lld_steer_k = (float)( STEER_MAX - STEER_MIN )/( CONTROL_MAX - CONTROL_MIN );
    lld_steer_b = ( STEER_MIN - lld_steer_k * CONTROL_MIN );
    /* Set initialization flag */

    isInitialized = true;

}


/**
 * @brief   Set power for driving motor
 * @param   inputPrc   Motor power value [-100 100]
 */
void lldControlSetDrMotorPower( controlValue_t inputPrc )
{
    inputPrc = CLIP_VALUE(inputPrc, CONTROL_MIN, CONTROL_MAX);

    if( inputPrc >= 0)
    {
        drDuty = lld_speed_forward_k * inputPrc + lld_speed_forward_b;
    }
    else
    {
        drDuty = lld_speed_back_k * inputPrc + lld_speed_back_b;
    }
    drDuty = CLIP_VALUE(drDuty, SPEED_MIN, SPEED_MAX);
    pwmEnableChannel( pwmDriver, drivePWMch, drDuty);
}

/**
 * @brief   Set power (in ticks) for driving motor
 * @param   drDuty   dutycycle for speed control
 */
void lldControlSetDrMotorRawPower( rawPwmValue_t dutyCycleSpeed)
{
    dutyCycleSpeed = CLIP_VALUE( dutyCycleSpeed, SPEED_MIN, SPEED_MAX );
    pwmEnableChannel( pwmDriver, drivePWMch, dutyCycleSpeed);
}

/**
 * @brief   Set power (in ticks) for steering motor
 * @param   steerDuty   dutycycle for steering control
 */
void lldControlSetSteerMotorRawPower( rawPwmValue_t dutyCycleSteer)
{
    dutyCycleSteer = CLIP_VALUE( dutyCycleSteer, STEER_MIN, STEER_MAX );
    pwmEnableChannel( pwmDriver, steerPWMch, dutyCycleSteer);
}

/**
 * @brief   Set power for steering motor
 * @param   inputPrc   Motor power value [-100 100]
 *                     central position = 0
 */
void lldControlSetSteerMotorPower( controlValue_t inputPrc )
{
    inputPrc = CLIP_VALUE(inputPrc, CONTROL_MIN, CONTROL_MAX);
    rawPwmValue_t drDuty = lld_steer_k * inputPrc + lld_steer_b;
    pwmEnableChannel( pwmDriver, steerPWMch, drDuty);
}
