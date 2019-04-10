#include <tests.h>
#include <chprintf.h>
#include <lld_charge_level.h>

int16_t Adc9V = 0;
int16_t Adc18V = 0;

void testChargeLevel (void)
{
   lldChargeLevelInit();

    while( true )
    {
    	//Adc9V =   lldChargeLevelGetAdcVal_STMPower();
    	//Adc18V =   lldChargeLevelGetAdcVal_Battery();
    	Adc18V = lldChargeLevelGetCharge_Battery();
    	Adc9V = lldChargeLevelGetCharge_STMPower();
    	chprintf( (BaseSequentialStream *)&SD7, " STMPower: %d   Battery: %d\r\n", Adc9V , Adc18V);
        chThdSleepMilliseconds( 500 );
    }
}
