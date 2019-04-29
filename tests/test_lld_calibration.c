#include <tests.h>
#include <lld_control.h>
#include <lld_steering_control.h>
#include <chprintf.h>

#define CLIP_VALUE(x, min, max) ((x) < (min) ? (min) :      \
                                 (x) > (max) ? (max) : (x))

/***  Serial configuration pins    ***/
static  SerialDriver    *SERIALdriver   = &SD7;

/*** Configuration structures ***/

static const SerialConfig sdcfg = {
    .speed  = 115200,
    .cr1 = 0, .cr2 = 0, .cr3 = 0
};

/***********************************************************/

void SerialInit( void )
{
    /*** serial pins configuration ***/
    palSetPadMode( GPIOE, 8, PAL_MODE_ALTERNATE(8) ); // TX
    palSetPadMode( GPIOE, 7, PAL_MODE_ALTERNATE(8) ); // RX
    sdStart( SERIALdriver, &sdcfg );
}


/////////////////////////////////////////////////////////////////////
static void gpt_callback ( GPTDriver *Tim5 );
static GPTDriver                     *Tim5 = &GPTD5;

static const GPTConfig gpt5cfg = {
  .frequency =  100000,
  .callback  =  gpt_callback,
  .cr2       =  0,
  .dier      =  0U
};

#define period_50ms         gpt5cfg.frequency/2

int32_t total_ticks                       = 0;
int32_t CalcTime                          = 0;
/*int32_t periodCheckPoint                  = 0;
int32_t last_periodCheckPoint             = 0;*/

static void gpt_callback (GPTDriver *gptd)
{
    gptd = gptd;
    total_ticks += period_50ms;
}

/////////////////////////////////////////////////////////////////////

#define MATLAB

int32_t result = 0;

void testCalibration( void )
{
	controlValue_t steer = 0;
    controlValue_t delta_steer = 100;

    //uint32_t DataArray[4] = {0, 0, 0, 0};
    //uint32_t DataLowPassArray[5] = {0, 0, 0, 0, 0};
    uint16_t AdcVal = 0;

    chSysInit();

    lldControlInit( );
    lldSteeringControlInit();

    gptStart(Tim5, &gpt5cfg);

#ifdef DEBUG_NUC
    debug_stream_init();
#endif

#ifdef MATLAB
    SerialInit();
#endif
    systime_t time = chVTGetSystemTime();
    while( 1 )
    {

      /* char rcv_data = sdGetTimeout( SERIALdriver, TIME_IMMEDIATE );
       switch ( rcv_data )
       {
           case 'q':   //
             steer += delta_steer;
               break;

           case 'a':   // Left steer
             steer = -delta_steer;
               break;

           case ' ':   // Reset
             steer = 0;
               break;

           default: ;
       }

    steer = CLIP_VALUE( steer, -100, 100 );
    lldControlSetSteerPower(steer);*/

    total_ticks = 0;
    gptStartContinuous(Tim5, period_50ms);

    result = chVTGetSystemTimeX();
    AdcVal = lldSteeringControlGetAdcVal_Kalman();

	CalcTime = total_ticks + gptGetCounterX(Tim5);

	//chprintf( (BaseSequentialStream *)&SD7, "%d \n", CalcTime );

    chprintf( (BaseSequentialStream *)&SD7, "Time:(%d / %d) - %d\n\r",
              (int)RTC2US( period_50ms * 1.0, CalcTime ), (int)CalcTime,
              (int)result );

    //dbgprintf("\n\r ", );
       time = chThdSleepUntilWindowed (time, time + MS2ST(500));
    }
}



/*DataArray[0]  = lldSteeringControlGetAdcVal();
DataArray[1] = lldSteeringControlGetAdcVal_filt();
DataArray[2]  = lldSteeringControlGetAdcVal_median();
DataArray[3]   = lldSteeringControlGetAdcVal_doublefilt();
sdWrite( &SD7, (uint8_t *)DataArray, sizeof( DataArray ) );

DataLowPassArray[0] = lldSteeringControlGetAdcVal();
DataLowPassArray[1] = lldSteeringControlGetAdcVal_Kalman09();
DataLowPassArray[2] = lldSteeringControlGetAdcVal_Kalman05();
DataLowPassArray[3] = lldSteeringControlGetAdcVal_Kalman01();
DataLowPassArray[4] = lldSteeringControlGetAdcVal_Kalman001();
sdWrite( &SD7, (uint8_t *)DataLowPassArray, sizeof( DataLowPassArray ) );*/




/*int16_t Average       = 0;
int16_t DoubleAverage = 0;
int16_t Median        = 0;
int16_t AdcVal        = 0;

int16_t LowPass09     = 0;
int16_t LowPass05     = 0;
int16_t LowPass01     = 0;
int16_t LowPass001    = 0;*/


/*AdcVal = lldSteeringControlGetAdcVal();
   Average = lldSteeringControlGetAdcVal_filt();
   DoubleAverage = lldSteeringControlGetAdcVal_doublefilt();
   Median = lldSteeringControlGetAdcVal_median();*/

   /*AdcVal = lldSteeringControlGetAdcVal();
   LowPass09 = lldSteeringControlGetAdcVal_Kalman09();
   LowPass05 = lldSteeringControlGetAdcVal_Kalman05();
   LowPass01 = lldSteeringControlGetAdcVal_Kalman01();
   LowPass001 = lldSteeringControlGetAdcVal_Kalman001();*/


   //dbgprintf("ADC(%d)\t Average(%d)\t DoubleAverage(%d)\t Median(%d)\t Steer(%d)\n\r ", AdcVal, Average, DoubleAverage, Median, steer);
   //dbgprintf("ADC(%d)\t LowPass09(%d)\t LowPass05(%d)\t LowPass01(%d)\t LowPass001(%d)\t Steer(%d)\n\r ", AdcVal, LowPass09, LowPass05, LowPass01, LowPass001, steer);
