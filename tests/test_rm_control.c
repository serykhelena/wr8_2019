#include <tests.h>
#include <rm_control.h>
#include <chprintf.h>

/***  PWM configuration pins    ***/

PWMConfig pwm5conf = { //PWM_period [s] = period / frequency
      .frequency = 1000000,
      .period    = 20000,
      .callback  = NULL,
      .channels  = {
                       {.mode = PWM_OUTPUT_ACTIVE_HIGH, .callback = NULL}, // Channel 1 is working CH1 = PE9
                       {.mode = PWM_OUTPUT_DISABLED,    .callback = NULL},
                       {.mode = PWM_OUTPUT_DISABLED,    .callback = NULL},
                       {.mode = PWM_OUTPUT_ACTIVE_HIGH, .callback = NULL}
                    },
      .cr2        = 0,
      .dier       = 0
  };

void testPWMInit( void )
{
    /*** PWM pins configuration ***/
    palSetLineMode( PAL_LINE( GPIOA, 0 ),  PAL_MODE_ALTERNATE(2) );
    palSetLineMode( PAL_LINE( GPIOA, 3 ),  PAL_MODE_ALTERNATE(2) );
    pwmStart( &PWMD5, &pwm5conf );

    pwmEnableChannel( &PWMD5, 0, 5000 );
    pwmEnableChannel( &PWMD5, 3, 3000 );
}

/*===========================================================================*/
/* Serial driver related.                                                    */
/*===========================================================================*/
static const SerialConfig sdcfg = {
  .speed = 115200,
  .cr1 = 0,
  .cr2 = 0,
  .cr3 = 0
};

void SerialInit2( void )
{
  sdStart( &SD7, &sdcfg );
  palSetPadMode( GPIOE, 8, PAL_MODE_ALTERNATE(8) );    // TX
  palSetPadMode( GPIOE, 7, PAL_MODE_ALTERNATE(8) );    // RX

  chprintf(((BaseSequentialStream *)&SD7), "TEST\r");
}

void TestRMControl(void)
{
  int test_speed = 0;
  int test_steer = 0;
  int count = 0;
  testPWMInit();
  ICUInit();
  SerialInit2();

  while (true)
  {
    test_steer = FetchSteer();
    test_speed = FetchSpeed();

    if (count == 100)
    {
      chprintf( (BaseSequentialStream *)&SD7, " Speed(%d)\t Steer(%d)\n\r ", test_speed, test_steer);
      count = 0;
    }
    else
    count++;

    chThdSleepMilliseconds( 1 );
  }
}
