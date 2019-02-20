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
  int test_speed = 1320;
  int test_steer = 1640;
  int tt_steer = 0;
  int tt_speed = 0;
  controlValue_t delta_spd = 10;
  controlValue_t delta_str = 10;
  int count = 0;
  testPWMInit();
  ICUInit();
  SerialInit2();

  while (true)
  {
  char rcv_data = sdGet( &SD7 );
  switch ( rcv_data )
        {
           case 'w':   // Positive speed
             test_speed += delta_spd;
               break;

           case 'z':   // Negative speed
             test_speed -= delta_spd;
               break;

           case 's':   // Right steer
             test_steer += delta_str;
               break;

           case 'a':   // Left steer
             test_steer -= delta_str;
               break;

           default:
             ;
       }

    pwmEnableChannel( &PWMD5, 0, test_steer );
    pwmEnableChannel( &PWMD5, 3, test_speed );

//      palSetLine( LINE_LED3 );

    tt_steer = FetchSteer();
    tt_speed = FetchSpeed();

//      palSetLine( LINE_LED2 );

//    if (count == 100)
//    {
//        palSetLine( LINE_LED3 );
      chprintf( (BaseSequentialStream *)&SD7, " PWM_Speed(%d)\t Speed_ICU(%d)\t PWM_Steer(%d)\t Steer(%d)\n\r ", test_speed, tt_speed, test_steer, tt_steer);
//      chprintf( (BaseSequentialStream *)&SD7, " PWM_Speed(%d)\t Speed(%d)\n\r ", test_speed, tt_speed);
//      count = 0;
//    }
//    else
//    count++;


      //  if ( rc_mode )
      //  {
      //    //Bunch rc with drives of speed & steer
      //    lldControlDrivingWheels(speed_rc);
      //    lldControlSteeringWheels(steer_rc);
      //  }

    chThdSleepMilliseconds( 1 );
  }
}
