#include <tests.h>
#include <lld_steering_control.h>


//int32_t AdcVal = 0;
//int16_t PosVal = 0;
//int16_t Angle  = 0;


uint32_t DataArray[4] = {0, 0, 0, 0};
uint32_t DataLowPassArray[5] = {0, 0, 0, 0, 0};


static const SerialConfig sdcfg = {
  .speed = 115200,
  .cr1 = 0,
  .cr2 = 0,
  .cr3 = 0
};

/////////////////////////////////////////////////////////////////////
static void gpt_callback ( GPTDriver *Tim5 );
static GPTDriver                     *Tim5 = &GPTD5;

static const GPTConfig gpt5cfg = {
  .frequency =  100000,
  .callback  =  gpt_callback,
  .cr2       =  0,
  .dier      =  0U
};

#define period_20ms         gpt5cfg.frequency/50
#define period_100ms        gpt5cfg.frequency/10
#define period_50ms         gpt5cfg.frequency/20

int32_t total_ticks                       = 0;
int32_t KalmanTime                        = 0;
int32_t periodCheckPoint                  = 0;
int32_t last_periodCheckPoint             = 0;

static void gpt_callback (GPTDriver *gptd)
{
    gptd = gptd;
    total_ticks += period_50ms;
}

/////////////////////////////////////////////////////////////////////

void sd_set(void)
{
    sdStart( &SD7, &sdcfg );
    palSetPadMode( GPIOE, 8, PAL_MODE_ALTERNATE(8) );    // TX
    palSetPadMode( GPIOE, 7, PAL_MODE_ALTERNATE(8) );    // RX
}

void testSteeringControl (void)
{
   sd_set();
   lldSteeringControlInit();
   /* Start working GPT driver in asynchronous mode */
   gptStart(Tim5, &gpt5cfg);
   gptStartContinuous(Tim5, period_50ms);
    while( true )
    {
    	/*last_periodCheckPoint = gptGetCounterX(Tim5);
    	AdcVal = lldSteeringControlGetAdcVal_Kalman ();
    	periodCheckPoint = gptGetCounterX(Tim5);
    	KalmanTime = total_ticks + periodCheckPoint - last_periodCheckPoint;
    	total_ticks = 0;
        //sdWrite( &SD7, (uint16_t *)&AdcVal, sizeof( AdcVal ) );
        chprintf( (BaseSequentialStream *)&SD7, "%d \n", AdcVal );*/



    	DataArray[0]  = lldSteeringControlGetAdcVal();
    	DataArray[1] = lldSteeringControlGetAdcVal_filt();
    	DataArray[2]  = lldSteeringControlGetAdcVal_median();
    	DataArray[3]   = lldSteeringControlGetAdcVal_doublefilt();
    	sdWrite( &SD7, (uint8_t *)DataArray, sizeof( DataArray ) );


    	/*DataLowPassArray[0] = lldSteeringControlGetAdcVal();
    	DataLowPassArray[1] = lldSteeringControlGetAdcVal_Kalman09();
    	DataLowPassArray[2] = lldSteeringControlGetAdcVal_Kalman05();
    	DataLowPassArray[3] = lldSteeringControlGetAdcVal_Kalman01();
    	DataLowPassArray[4] = lldSteeringControlGetAdcVal_Kalman001();
    	sdWrite( &SD7, (uint8_t *)DataLowPassArray, sizeof( DataLowPassArray ) );*/

        chThdSleepMilliseconds( 10 );
    }
}



