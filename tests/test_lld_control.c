#include <tests.h>
#include <lld_control.h>
#include <chprintf.h>

/***  Serial configuration pins    ***/
static  SerialDriver    *SERIALdriver   = &SD7;

/*** Configuration structures ***/

static const SerialConfig sdcfg = {
    .speed  = 115200,
    .cr1 = 0, .cr2 = 0, .cr3 = 0
};

/***********************************************************/

void SerialInit( void )
{
    /*** serial pins configuration ***/
    palSetPadMode( GPIOE, 8, PAL_MODE_ALTERNATE(8) ); // TX
    palSetPadMode( GPIOE, 7, PAL_MODE_ALTERNATE(8) ); // RX
    sdStart( SERIALdriver, &sdcfg );

    chprintf(((BaseSequentialStream *)SERIALdriver), "TEST\r");
}

void testDriverControlRoutine( void )
{
    controlValue_t speed = 0;
    controlValue_t delta_speed = 5;

    controlValue_t steer = 0;
    controlValue_t delta_steer = 5;

    lldControlInit( );
    debug_stream_init();
//    SerialInit();

    while( 1 )
    {

      char rcv_data = sdGet( &SD3 );
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
      speed = CLIP_VALUE( speed, -100, 100 );
      lldControlDrivingWheels(speed);
      steer = CLIP_VALUE( steer, -100, 100 );
      lldControlSteeringWheels(steer);
//      chprintf( (BaseSequentialStream *)SERIALdriver, "\t Speed(%d) Steer(%d)\n\r ", speed, steer);
      dbgprintf("\t Speed(%d) Steer(%d)\n\r ", speed, steer);
      chThdSleepMilliseconds(100);
    }
}
