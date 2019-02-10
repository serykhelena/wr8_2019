#include <tests.h>
#include <chprintf.h>
#include <lld_charge_level.h>


void testChargeLevel (void)
{
   lldChargeLevelInit();

    while( true )
    {
    	lldChargeLevelDisplay();
        chThdSleepMilliseconds( 1 );
    }
}
