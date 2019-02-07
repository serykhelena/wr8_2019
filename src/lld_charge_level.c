#include <tests.h>
#include <lld_charge_level.h>


/*** Variable configuration ***/

static bool     isInitialized       = false;

static float    highLvlVoltage      = 3000;// 3 B
static float    lowLvlVoltage       = 0;   // 0 B
static float    maxLvl              = 2800;// 6 B
static float    minLvl              = 2000;// 8.4 B

static uint16_t     VoltDiap        = 0;
static uint16_t     Display         = 0;
static uint16_t     chargeStatus    = 0;

static float        chargePart      = 0;
static float        displayPart     = 0;

void lldChargeLevelInit (void)
{
    InitAdc();
    
    VoltDiap = highLvlVoltage - lowLvlVoltage;
    Display = maxLvl - minLvl;
    chargePart = 100 / VoltDiap;
    displayPart = 10 / Display;
    
    isInitialized = true;
}

int16_t lldChargeLevelGetAdcVal (void)
{
    if ( !isInitialized )
	    return false;
        
	return GetAdcVal();
}


int16_t lldChargeLevelGetCharge (void)
{
    if ( !isInitialized )
        return false; 
    
    currentCharge = chargePart * (GetAdcVal() - lowLvlVoltage);
    
    return currentCharge;
}

int8_t lldChargeLevelDisplay (void)
{
    if ( !isInitialized )
        return false; 
    
    int8_t diodsQuantity = 0;
    int16_t currentVal = GetAdcVal();
   
    if (currentVal <=  maxLvl && currentVal >=  minLvl)
    {
        diodsQuantity = (currentVal - minLvl) * displayPart;
    }
    return (int)diodsQuantity;    
}

