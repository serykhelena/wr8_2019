#include <tests.h>
#include <encoder.h>


static const SerialConfig sdcfg = {
  .speed = 115200,
  .cr1 = 0, .cr2 = 0, .cr3 = 0
};

void testEncoderRoutine( void )
{
    sdStart( &SD7, &sdcfg );
    palSetPadMode( GPIOE, 8, PAL_MODE_ALTERNATE(8) );   // TX
    palSetPadMode( GPIOE, 7, PAL_MODE_ALTERNATE(8) );   // RX

    encoderInit( );

    rawEncoderValue_t   ticks   = 0;
    encoderValue_t      revs    = 0;

    chprintf( (BaseSequentialStream *)&SD7, "TEST ENCODER\n\r" );

    while( 1 )
    {
        ticks   = getEncoderRawTickNumber( );
        revs    = getEncoderRevNumber( );

        chprintf( (BaseSequentialStream *)&SD7, "Ticks:(%d)\n\r\t Revs:(%d)\n\r\t", ticks, revs );
        chThdSleepMilliseconds( 10 );
    }

}
