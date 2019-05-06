#include <tests.h>
#include <lld_control.h>
#include <lld_steering_control.h>
#include <chprintf.h>


#define DEBUG_NUC
//#define MATLAB


#define GPT_USE // not working
//#define TM_USE


#ifdef MATLAB
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
#endif

/////////////////////////////////////////////////////////////////////
#ifdef GPT_USE
static void gpt_callback ( GPTDriver *Tim5 );
static GPTDriver                     *Tim5 = &GPTD5;

int32_t gpt_ticks                       = 0;
int32_t total_time                      = 0;

static const GPTConfig gpt5cfg = {
  .frequency =  100000,
  .callback  =  gpt_callback,
  .cr2       =  0,
  .dier      =  0U
};

#define period_50ms         gpt5cfg.frequency/2

static void gpt_callback (GPTDriver *gptd)
{
    gptd = gptd;
    gpt_ticks += period_50ms;
}

#define SYSTEM_FREQUENCY   gpt5cfg.frequency

#endif
/////////////////////////////////////////////////////////////////////



#ifdef TM_USE

#define SYSTEM_FREQUENCY   216000000UL
static time_measurement_t  processing_time;

#endif


uint16_t Value = 0;


double test_function( void )
{
    float var;
    for( uint32_t g = 0; g < 400000; g++)
    {
        var = chVTGetSystemTimeX();
        var *= 2;
        var *= chVTGetSystemTimeX();
        var /= chVTGetSystemTimeX();
        var = var - 5.2;
    }

    return var;
}

double result;

/*
 * COMMENTS
 */
void testCalibration( void )
{

    lldSteeringControlInit();
#ifdef GPT_USE
    gptStart(Tim5, &gpt5cfg);
#endif
#ifdef TM_USE
    chTMObjectInit( &processing_time );
#endif

#ifdef DEBUG_NUC
    debug_stream_init();
#endif

#ifdef MATLAB
    SerialInit();
#endif

    systime_t time = chVTGetSystemTime();
    while( 1 )
    {
#ifdef TM_USE
        chTMStartMeasurementX( &processing_time );
#endif
#ifdef GPT_USE
        gpt_ticks = 0;
        gptStartContinuous(Tim5, period_50ms);
#endif

        result = test_function();
//        Value = lldSteeringControlGetAdcVal_Kalman();

#ifdef GPT_USE
	    total_time = gpt_ticks + gptGetCounterX(Tim5);
#endif
#ifdef TM_USE
	    chTMStopMeasurementX( &processing_time );

#endif
#ifdef MATLAB
	    /*You send string to Matlab O_o
	     *Are you sure that it is a good idea? */
        chprintf( (BaseSequentialStream *)&SD7, "Time:(%d / %d) - %d\n\r",
              (int)RTC2US( period_50ms * 1.0, total_time ), (int)total_time,
              (int)result );
#endif
#ifdef DEBUG_NUC

  #ifdef GPT_USE
        dbgprintf( "T:%d (us)\t/%d(ticks)\n\r",
                   (int)RTC2US( SYSTEM_FREQUENCY * 1.0, total_time ),
                   gpt_ticks );
//                   total_time);
  #endif
  #ifdef TM_USE
        dbgprintf( "%d / Best time:(%d)\tWorst time:(%d)\n\r",
                   (int)result,
                   RTC2US( SYSTEM_FREQUENCY, processing_time.best ),
                   RTC2US( SYSTEM_FREQUENCY, processing_time.worst ) );

  #endif

#endif
       time = chThdSleepUntilWindowed (time, time + MS2ST(200));
    }
}


