#include <tests.h>
#include <lld_steering_control.h>


int16_t AdcVal = 0;
int16_t PosVal = 0;
int16_t Angle  = 0;

static const SerialConfig sdcfg = {
  .speed = 115200,
  .cr1 = 0,
  .cr2 = 0,
  .cr3 = 0
};

void sd_set(void)
{
    sdStart( &SD7, &sdcfg );
    palSetPadMode( GPIOE, 8, PAL_MODE_ALTERNATE(8) );    // TX
    palSetPadMode( GPIOE, 7, PAL_MODE_ALTERNATE(8) );    // RX
}


void testSteeringControl (void)
{
   sd_set();
   lldSteeringControlInit();
    while( true )
    {
        AdcVal = lldSteeringControlGetAdcVal();
    	//AdcVal = lldSteeringControGetAdcPos_filt();
        sdWrite( &SD7, (uint16_t *)&AdcVal, sizeof( AdcVal ) );
        //chprintf( (BaseSequentialStream *)&SD7, "%d \n", AdcVal );
        chThdSleepMilliseconds( 10 );
    }
}

