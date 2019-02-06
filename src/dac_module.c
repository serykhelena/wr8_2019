#include <tests.h>
#include <dac_module.h>


/*** DAC configuration pins      ***/
#define dacLine         PAL_LINE( GPIOA, 4 )

static  DACDriver       *dacDriver      = &DACD1;

/* Can be set or set to 0 to calculate from <speedMinVoltage> */
static int32_t  speedMinDACValue    = 0;

#define VOLTAGE_2_DAC(v)  ((v) / 3.3 * 4095)

static const DACConfig dac_cfg = {
    /* Initial value of DAC out */
    .init         = 0,
    /*
     * Mode of DAC:
     *      DAC_DHRM_12BIT_RIGHT - 12 bit with right alignment
     *      DAC_DHRM_12BIT_LEFT  - 12 bit with left alignment
     *      DAC_DHRM_8BIT_RIGHT  - 8 bit no alignment (half of dacsample_t [uint16_t] type)
     */
    .datamode     = DAC_DHRM_12BIT_RIGHT,
    /* Direct register set, future used for triggering DAC */
    .cr           = 0
};

static bool         isInitialized       = false;
static float        speedConvRate       = 0.0;

void dacInit( void )
{
    /*
    * DAC has two channels
    * Datasheet p69, PA4 - DACout1, PA5 - DACout2
    * Pin configuration for 1st channel
    */
    palSetLineMode( dacLine, PAL_MODE_INPUT_ANALOG );

    /* Start DAC driver with configuration */
    dacStart( dacDriver, &dac_cfg );

    /* Calculate some parameters */

    if ( speedMaxDACValue == 0 )
    {
        speedMaxDACValue = VOLTAGE_2_DAC( speedMaxVoltage );
    }

    if ( speedMinDACValue == 0 )
    {
        speedMinDACValue = VOLTAGE_2_DAC( speedMinVoltage );
    }

    speedConvRate = (speedMaxDACValue - speedMinDACValue) / 100.0;

    /* Set initialization flag */

    isInitialized = true;
}
