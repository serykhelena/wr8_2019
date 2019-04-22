#include <tests.h>
#include <lld_control.h>
#include <chprintf.h>
#include <common.h>

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

    dbgprint(((BaseSequentialStream *)SERIALdriver), "TEST\r");
}

void testDriverControlRoutine( void )
{
    controlValue_t speed = 0;
    controlValue_t delta_speed = 1;

    controlValue_t steer = 0;
    controlValue_t delta_steer = 1;

    lldControlInit( );
    SerialInit();

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

    dbgprint(((BaseSequentialStream *)SERIALdriver), "\t Speed(%d) Steer(%d)\n\r ");
    dbgprint(((BaseSequentialStream *)SERIALdriver), "\t Speed(%d) Steer(%d)\n\r ", speed, steer);

    systime_t chThdSleepUntilWindowed(systime_t prev, systime_t next);
    }
}
