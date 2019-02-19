#include <tests.h>
#include <lld_charge_level.h>
#include <common.h>


/*** Variable configuration ***/

static bool     isInitialized       = false;

static float    maxLvl_STMPower     = 3500;// 6 B -> change on 6.4 V
static float    minLvl_STMPower     = 2500;// 8.4 B

static float    maxLvl_Battery      = 3500;// 12.8 B define adc val
static float    minLvl_Battery      = 2500;// 16.8 B define adc val

static float    VoltDiap_STMPower   = 0;
static float    VoltDiap_Battery    = 0;

static float    chargePart9V        = 0;
static float    chargePart18V       = 0;



void lldChargeLevelInit (void)
{
    InitAdc3();
    
    VoltDiap_STMPower = maxLvl_STMPower - minLvl_STMPower;
    VoltDiap_Battery  = maxLvl_Battery  - minLvl_Battery;
    chargePart9V =  100 / VoltDiap_STMPower;
    chargePart18V = 100 / VoltDiap_Battery;

    isInitialized = true;
}

int16_t lldChargeLevelGetAdcVal_STMPower (void)
{
    if ( !isInitialized )
	    return false;
        
	return GetAdcVal_STMPower();
}

int16_t lldChargeLevelGetAdcVal_Battery (void)
{
    if ( !isInitialized )
	    return false;

	return GetAdcVal_Battery();
}

int16_t lldChargeLevelGetCharge_STMPower (void)
{
    if ( !isInitialized )
        return false;

    int16_t currentCharge = 0;
    
    currentCharge = chargePart9V * (GetAdcVal_STMPower() - minLvl_STMPower);
    
    return currentCharge;
}

int16_t lldChargeLevelGetCharge_Battery (void)
{
    if ( !isInitialized )
        return false;

    int16_t currentCharge = 0;

    currentCharge = chargePart18V * (GetAdcVal_Battery() - minLvl_Battery);

    return currentCharge;
}

