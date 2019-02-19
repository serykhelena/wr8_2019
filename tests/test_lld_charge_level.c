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
    	Adc9V =  lldChargeLevelGetCharge_STMPower();
    	Adc18V = lldChargeLevelGetCharge_Battery();

    	chprintf( (BaseSequentialStream *)&SD7, " STMPower: %d   Battery: %\n", Adc9V , Adc18V);
        chThdSleepMilliseconds( 500 );
    }
}
