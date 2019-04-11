#include <tests.h>
#include <lld_control.h>
#include <lld_steering_control.h>
#include <chprintf.h>

#define CLIP_VALUE(x, min, max) ((x) < (min) ? (min) :      \
                                 (x) > (max) ? (max) : (x))

/***  Serial configuration pins    ***/
static  SerialDriver    *SERIALdriver   = &SD3;

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

void testCalibration( void )
{
    controlValue_t speed = 0;
    controlValue_t delta_speed = 5;

    controlValue_t steer = 0;
    controlValue_t delta_steer = 5;

    int16_t AdcVal = 0;

    lldControlInit( );
    lldSteeringControlInit();
    debug_stream_init();
//    SerialInit();


    while( 1 )
    {
       char rcv_data = sdGetTimeout( SERIALdriver, TIME_IMMEDIATE );
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

           default: ;
       }

    speed = CLIP_VALUE( speed, -100, 100 );
    lldControlSetDrivePower(speed);

    steer = CLIP_VALUE( steer, -100, 100 );
    lldControlSetSteerPower(steer);
    //AdcVal = lldSteeringControlGetAdcVal();
    AdcVal = lldSteeringControlGetAdcVal_doublefilt();

    dbgprintf("Speed(%d)\tSteer(%d)\tAdcVal(%d)\n\r ", speed, steer, AdcVal);
    chThdSleepMilliseconds(100);
    }
}
