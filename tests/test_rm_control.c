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

void TestRMControl(void)
{

  int test_speed = 1515;
  int test_steer = 1417;
  int tt_steer = 0;
  int tt_speed = 0;
  controlValue_t delta_spd = 1;
  controlValue_t delta_str = 1;
  testPWMInit();
  ICUInit();

  lldControlInit( );
  debug_stream_init( );

  systime_t time = chVTGetSystemTime(); // Current system time.

  dbgprintf("TEST\r");

  while (true)
  {
#if 0
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

    test_steer = CLIP_VALUE( test_steer, 1200, 2080 );
    pwmEnableChannel( &PWMD5, 0, test_steer );
    test_speed = CLIP_VALUE( test_speed, 1240, 1600 );
    pwmEnableChannel( &PWMD5, 3, test_speed );

    tt_steer = FetchSteer();
    tt_speed = FetchSpeed();

    chprintf( (BaseSequentialStream *)&SD7, " PWM_Speed(%d)\t Speed_ICU(%d)\t PWM_Steer(%d)\t Steer(%d)\n\r ",
              test_speed, tt_speed, test_steer, tt_steer);

    //Bunch rc with drives of speed & steer
    lldControlDrivingWheels(tt_speed);
    lldControlSteeringWheels(tt_steer);
#endif

    uint32_t            test_raw_steer  = icuRawSteerInput( );
    uint32_t            test_raw_speed  = icuRawSpeedInput( );
    controlValueICU_t   test_prct_steer =   FetchSteer( );
    controlValueICU_t   test_prct_speed =   FetchSpeed( );

    lldControlSteeringWheels( test_prct_steer );
    lldControlDrivingWheels(test_prct_speed);

    dbgprintf("RAW_STEER:(%d)\tSTEER_PRCT:(%d)\tRAW_SPEED:(%d)\tSPEED_PRCT:(%d)\n\r",
          test_raw_steer, test_prct_steer, test_raw_speed,test_prct_speed );


    time = chThdSleepUntilWindowed(time, time + MS2ST(100));
  }
}
