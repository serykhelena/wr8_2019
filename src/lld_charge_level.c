#include <tests.h>
#include <lld_charge_level.h>
#include <common.h>


/*** Variable configuration ***/

static bool     isInitialized       = false;

static float    maxLvl_STMPower     = 3456;
static float    minLvl_STMPower     = 2630;

static float    maxLvl_Battery      = 3458;
static float    minLvl_Battery      = 2635;

static float    VoltDiap_STMPower   = 0;
static float    VoltDiap_Battery    = 0;

static float    chargePart9V        = 0;
static float    chargePart18V       = 0;


/*
 * @brief   Initialize charge level module
 */
void lldChargeLevelInit (void)
{
	InitAdc3();
    
	/* define ADC values ranges for STMPower(9V) and Battery(18V) */
    VoltDiap_STMPower = maxLvl_STMPower - minLvl_STMPower;
    VoltDiap_Battery  = maxLvl_Battery  - minLvl_Battery;
    /* define coefficients for converting adc values ​​to percents */
    chargePart9V =  100 / VoltDiap_STMPower;
    chargePart18V = 100 / VoltDiap_Battery;

    isInitialized = true;
}

/*
 * @brief   Get ADC № 3 value from A3(AN9)
 * @return  ADC value from 0 to 4095 equal voltage for STM power (2cell-battery)
 */
int16_t lldChargeLevelGetAdcVal_STMPower (void)
{
    if ( !isInitialized )
	    return false;
        
	return GetAdcVal_STMPower();
}

/*
 * @brief   Get ADC № 3 value from A4(AN15)
 * @return  ADC value from 0 to 4095 equal voltage from main battery (4cell-battery)
 */
int16_t lldChargeLevelGetAdcVal_Battery (void)
{
    if ( !isInitialized )
	    return false;

	return GetAdcVal_Battery();
}

/*
 * @brief 	Get current 2cell-battery power
 * @return  2cell-battery percentage from 0 to 100
 */
int16_t lldChargeLevelGetCharge_STMPower (void)
{
    if ( !isInitialized )
        return false;

    int16_t currentCharge = 0;
    
    currentCharge = chargePart9V * (GetAdcVal_STMPower() - minLvl_STMPower);

    /* values only in the range from 0 to 100 */
    if (currentCharge < 0)
    {
    	currentCharge = 0;
    }
    else if (currentCharge > 100)
    {
    	currentCharge = 100;
    }
    
    return currentCharge;
}

/*
 * @brief 	Get current 4cell-battery power
 * @return  4cell-battery percentage from 0 to 100
 */
int16_t lldChargeLevelGetCharge_Battery (void)
{
    if ( !isInitialized )
        return false;

    int16_t currentCharge = 0;

    currentCharge = chargePart18V * (GetAdcVal_Battery() - minLvl_Battery);

    /* values only in the range from 0 to 100 */
    if (currentCharge < 0)
    {
    	currentCharge = 0;
    }
    else if (currentCharge > 100)
    {
    	currentCharge = 100;
    }

    return currentCharge;
}

