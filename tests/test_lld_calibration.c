#include <tests.h>
#include <lld_control.h>
#include <lld_steering_control.h>
#include <chprintf.h>


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

static void gpt_callback (GPTDriver *gptd)
{
    gptd = gptd;
    total_ticks += period_50ms;
}

/////////////////////////////////////////////////////////////////////

#define MATLAB

int32_t result = 0;
uint16_t Value = 0;

void testCalibration( void )
{

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
        total_ticks = 0;
        gptStartContinuous(Tim5, period_50ms);

        result = chVTGetSystemTimeX();

        Value = lldSteeringControlGetAdcVal_Kalman();

	    CalcTime = total_ticks + gptGetCounterX(Tim5);

        chprintf( (BaseSequentialStream *)&SD7, "Time:(%d / %d) - %d\n\r",
              (int)RTC2US( period_50ms * 1.0, CalcTime ), (int)CalcTime,
              (int)result );

       time = chThdSleepUntilWindowed (time, time + MS2ST(200));
    }
}


