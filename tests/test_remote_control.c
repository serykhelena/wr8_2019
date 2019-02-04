#include <tests.h>
#include <remote_control.h>

static const SerialConfig sdcfg = {
  .speed = 115200,
  .cr1 = 0, .cr2 = 0, .cr3 = 0
};

void testRemoteControlRoutine( void )
{
    sdStart( &SD7, &sdcfg );
    palSetPadMode( GPIOE, 8, PAL_MODE_ALTERNATE(8) );   // TX
    palSetPadMode( GPIOE, 7, PAL_MODE_ALTERNATE(8) );   // RX

    remoteControlInit( 0 );

    icuValue_t  rc_speed    =   0;
    icuValue_t  rc_steer    =   0;

    chprintf( (BaseSequentialStream *)&SD7, "RC TEST\n\r" );
    systime_t time = chVTGetSystemTimeX();

    while( 1 )
    {
        time += MS2ST(20); // Next deadline

        rc_speed = rcSpeedControlSignalShow( );
        rc_steer = rcSteerControlSignalShow( );

        chprintf( (BaseSequentialStream *)&SD7, "Speed:(%d)\tStter:(%d)\n\r\t",
                  rc_speed, rc_steer );

        chThdSleepUntil(time);
    }
}

