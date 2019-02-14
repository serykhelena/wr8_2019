#include <tests.h>
#include <chprintf.h>
#include <lld_charge_level.h>

int16_t Adc = 0;

void testChargeLevel (void)
{
   lldChargeLevelInit();

    while( true )
    {
    	//Adc = lldChargeLevelGetAdcVal();
    	Adc = lldChargeLevelGetCharge();
    	//Adc = lldChargeLevelGetAdcVal_Kalman();
    	chprintf( (BaseSequentialStream *)&SD7, "%d  %\n", Adc );
        chThdSleepMilliseconds( 500 );
    }
}
