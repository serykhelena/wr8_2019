#include <ch.h>
#include <tests.h>
#include <lld_wheel_rotate.h>

#include <chprintf.h>


int16_t AdcVal = 0;
int16_t PosVal = 0;
int16_t Angle  = 0;

static const SerialConfig sdcfg = {
  .speed = 115200,
  .cr1 = 0,
  .cr2 = 0, //USART_CR2_LINEN,
  .cr3 = 0
};

void sd_set(void)
{
    sdStart( &SD7, &sdcfg );
    palSetPadMode( GPIOE, 8, PAL_MODE_ALTERNATE(8) );    // TX
    palSetPadMode( GPIOE, 7, PAL_MODE_ALTERNATE(8) );    // RX
}

void testFrontWheels (void)
{
   sd_set();
   lldServInit();

    while( true )
    {
    	AdcVal = lldGetFrontWheelAdcPos();
        PosVal = lldGetFrontWheelVal();
        Angle  = lldGetFrontWheelAngle();

        /*chprintf((BaseSequentialStream *)&SD7, "ADC = %d \n\r", AdcVal );
        chprintf(((BaseSequentialStream *)&SD7, "Position = %d \n\r", PosVal);
        chprintf((BaseSequentialStream *)&SD7, "Angle = %d \n\r", Angle);*/

        chprintf( (BaseSequentialStream *)&SD7, "ADC: %d / Pos: %04d\n\r", AdcVal, PosVal );
        chThdSleepMilliseconds( 200 );
    }
}
