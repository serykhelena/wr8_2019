#include <tests.h>
#include <lld_control.h>

#include <ch.h>
#include <hal.h>
#include <chprintf.h>

#define CLIP_VALUE(x, min, max) ((x) < (min) ? (min) :      \
                                 (x) > (max) ? (max) : (x))

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

/***  Serial configuration pins    ***/
static  SerialDriver    *serialDriver   = &SD7;

/*** Configuration structures ***/

static const SerialConfig sdcfg = {
    .speed  = 115200,
    .cr1 = 0, .cr2 = 0, .cr3 = 0
};

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

int a = 0;

void SerialInit( void )
{
//    if ( isInitialized )
//        return;
    /*** serial pins configuration ***/
    palSetPadMode( GPIOE, 8, PAL_MODE_ALTERNATE(8) ); // TX
    palSetPadMode( GPIOE, 7, PAL_MODE_ALTERNATE(8) ); // RX
    sdStart( &SD7, &sdcfg );

    chprintf(((BaseSequentialStream *)serialDriver), "TEST\r");

    /* Set initialization flag */
//    isInitialized = true;
}

void lldControlInit( void )
{
//    if ( isInitialized )
//        return;
    /*** PWM pins configuration ***/
    palSetLineMode( pwm1LineCh0,  PAL_MODE_ALTERNATE(1) );
    palSetLineMode( pwm1LineCh1,  PAL_MODE_ALTERNATE(1) );
    pwmStart( pwmDriver, &pwm1conf );

    /* Set initialization flag */
//  isInitialized = true;
}

void lldControlSetMotor(a)
{
    a = CLIP_VALUE( a, speed_min, speed_max );
    if (a >= 0)
    {
      speed_dutyK = 0.5;
      speed_dutyB = 1400;
    }
    else
    {
      speed_dutyK = 0.8;
      speed_dutyB = 1320;
    }
    int speedDuty = a * speed_dutyK + speed_dutyB;


    pwmEnableChannel( pwmDriver, speedPWMch, speedDuty );
    chprintf(((BaseSequentialStream *)serialDriver), "DUTY(%d)\r", speedDuty );
}

void lldControlSetMotor2(a)
{
    pwmEnableChannel( pwmDriver, speedPWMch, 1480+a );
}

void lldControlSetSteer(a)
{
    a = CLIP_VALUE( a, steer_min, steer_max );
    int steerDuty = a * steer_dutyK + steer_dutyB;

    pwmEnableChannel( pwmDriver, steerPWMch, steerDuty );
}

void lldControlSetSteer2(int v)
{
    pwmEnableChannel( pwmDriver, steerPWMch, 1510+v );
}

int main(void)
{
    int speed = 0;          //for serial test
    int delta_speed = 10;   //

    int steer = 0;
    int delta_steer = 10;

    chSysInit();
    halInit();

    lldControlInit();
    SerialInit();

    while (1)
    {
     char rcv_data = sdGet( serialDriver );
             switch ( rcv_data )
             {
                 case 'w':   // Positive speed
                   speed += delta_speed;
                     break;

                 case 'z':   // Negative speed
                   speed -= delta_speed;
                     break;

                 case 's':   // Right steer
                   steer += delta_steer;
                     break;

                 case 'a':   // Left steer
                   steer -= delta_steer;
                     break;

                 default:
                   ;
             }

     lldControlSetMotor(speed);
     lldControlSetSteer(steer);
     chprintf( (BaseSequentialStream *)serialDriver, "\t Speed(%d) Steer(%d)\n\r ", speed, steer);
     chThdSleepMilliseconds( 100 );

    }
}
