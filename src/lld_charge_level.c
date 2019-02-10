#include <tests.h>
#include <lld_charge_level.h>
#include <common.h>


/*** Variable configuration ***/

static bool     isInitialized       = false;

//static float    highLvlVoltage      = 3000;// 3 B
//static float    lowLvlVoltage       = 0;   // 0 B
static float    maxLvl              = 2800;// 6 B
static float    minLvl              = 2000;// 8.4 B

static uint16_t     VoltDiap        = 0;

static float        chargePart      = 0;
static float        displayPart     = 0;

void lldChargeLevelInit (void)
{
    InitAdc();
    
    VoltDiap = maxLvl - minLvl;
    chargePart = 100 / VoltDiap;
    displayPart = 10 / VoltDiap;
    
    isInitialized = true;
}

int16_t lldChargeLevelGetAdcVal (void)
{
    if ( !isInitialized )
	    return false;
        
	return GetAdcValVoltage();
}


int16_t lldChargeLevelGetCharge (void)
{
    if ( !isInitialized )
        return false;

    int16_t currentCharge = 0;
    
    currentCharge = chargePart * (GetAdcValVoltage() - minLvl);
    
    return currentCharge;
}

int8_t lldChargeLevelDiodsQuantity (void)
{
    if ( !isInitialized )
        return false; 
    
    int8_t diodsQuantity = 0;
    int16_t currentVal = GetAdcValVoltage();
   
    if (currentVal <=  maxLvl && currentVal >=  minLvl)
    {
        diodsQuantity = (currentVal - minLvl) * displayPart;
    }
    return (int)diodsQuantity;    
}

void lldChargeLevelDisplay (void)
{
    /*PC_14, PC_15, PH_0, PH_1, PE_3 */
	int8_t Diods = lldChargeLevelDiodsQuantity();

	palSetLineMode( PAL_LINE(GPIOC,14), PAL_MODE_OUTPUT_PUSHPULL);
	palSetLineMode( PAL_LINE(GPIOC,15), PAL_MODE_OUTPUT_PUSHPULL);
	palSetLineMode( PAL_LINE(GPIOH,0), PAL_MODE_OUTPUT_PUSHPULL);
	palSetLineMode( PAL_LINE(GPIOH,1), PAL_MODE_OUTPUT_PUSHPULL);
	palSetLineMode( PAL_LINE(GPIOE,3), PAL_MODE_OUTPUT_PUSHPULL);

	if (Diods % 2 == 1)
	{
        Diods += 1;
	}

	switch (Diods)
	{
	    case (2):
		    palSetLine(PAL_LINE(GPIOC,14));
	        break;
	    case (4):
            palSetLine(PAL_LINE(GPIOC,14));
		    palSetLine(PAL_LINE(GPIOC,15));
            break;
	    case (6):
            palSetLine(PAL_LINE(GPIOC,14));
            palSetLine(PAL_LINE(GPIOC,15));
		    palSetLine(PAL_LINE(GPIOH,0));
            break;
	    case (8):
            palSetLine(PAL_LINE(GPIOC,14));
            palSetLine(PAL_LINE(GPIOC,15));
            palSetLine(PAL_LINE(GPIOH,0));
		    palSetLine(PAL_LINE(GPIOH,1));
            break;
	    case (10):
            palSetLine(PAL_LINE(GPIOC,14));
            palSetLine(PAL_LINE(GPIOC,15));
            palSetLine(PAL_LINE(GPIOH,0));
            palSetLine(PAL_LINE(GPIOH,1));
		    palSetLine(PAL_LINE(GPIOE,3));
            break;
	    default:
	        break;
	}
}

