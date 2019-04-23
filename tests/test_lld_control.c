#include <tests.h>
#include <lld_control.h>
#include <chprintf.h>
#include <common.h>

/***  Serial configuration pins    ***/
static  SerialDriver    *SERIALdriver   = &SD3;

void testDriverControlRoutine( void )
{
    controlValue_t speed = 0;
    controlValue_t delta_speed = 1;

    controlValue_t steer = 0;
    controlValue_t delta_steer = 1;

    debug_stream_init( );
    lldControlInit( );

    systime_t time = chVTGetSystemTime(); // Current system time

    while( 1 )
    {
       char rcv_data = sdGet( SERIALdriver );
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

           case ' ':
             steer = 0;
             speed = 0;
               break;

           default: ;
       }

    speed = CLIP_VALUE( speed, -100, 100 );
    lldControlSetDrivePower(speed);

    steer = CLIP_VALUE( steer, -100, 100 );
    lldControlSetSteerPower(steer);

    dbgprintf("\t Speed(%d) Steer(%d)\n\r ", speed, steer);

    time = chThdSleepUntilWindowed(time, time+ MS2ST(100));
    }
}
