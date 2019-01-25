#include <tests.h>
#include <lld_control.h>

/***************************************************/

static const SerialConfig sdcfg = {
  .speed = 115200,
  .cr1 = 0, .cr2 = 0, .cr3 = 0
};


void testRawDrivingWheelControlRoutine( void )
{

    sdStart( &SD7, &sdcfg );
    palSetPadMode( GPIOE, 8, PAL_MODE_ALTERNATE(8) );   // TX
    palSetPadMode( GPIOE, 7, PAL_MODE_ALTERNATE(8) );   // RX

    lldControlInit();

    controlValue_t  speed_values_delta  = 5;
    controlValue_t  speed_value         = 0;
    controlValue_t  speed_duty          = 0;

    chprintf( (BaseSequentialStream *)&SD7, "TEST\n\r" );

    while ( 1 )
    {
        char rcv_data = sdGet( &SD7 );
        switch ( rcv_data )
        {
            case 'a':   // Positive speed
            speed_value += speed_values_delta;
            break;

            case 's':   // Negative speed
            speed_value -= speed_values_delta;
            break;

            default:
                ;
        }

        speed_value = CLIP_VALUE( speed_value, -380, 380 );

        speed_duty = lldControlSetDrMotorRawPower( speed_value );

        chprintf( (BaseSequentialStream *)&SD7, "Powers:\n\r\tDelta(%d)\n\r\tDuty(%d)\n\r\t",
                     speed_value, speed_duty );
        chThdSleepMilliseconds( 100 );
    }

}
