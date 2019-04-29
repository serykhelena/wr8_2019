#include <tests.h>
#include <lld_steering_control.h>
#include <chprintf.h>

int32_t AdcVal = 0;


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


#define period_50ms         gpt5cfg.frequency/20

static void gpt_callback (GPTDriver *gptd)
{
    gptd = gptd;
}

/////////////////////////////////////////////////////////////////////

void SerialInit(void)
{
    sdStart( &SD7, &sdcfg );
    palSetPadMode( GPIOE, 8, PAL_MODE_ALTERNATE(8) );    // TX
    palSetPadMode( GPIOE, 7, PAL_MODE_ALTERNATE(8) );    // RX
}

#define MATLAB
//#define DEBUG_NUC

void testSteeringControl (void)
{
    #ifdef DEBUG_NUC
        debug_stream_init();
    #endif

    #ifdef MATLAB
        SerialInit();
    #endif

   lldSteeringControlInit();
   /* Start working GPT driver in asynchronous mode */
   gptStart(Tim5, &gpt5cfg);
   gptStartContinuous(Tim5, period_50ms);

   systime_t time = chVTGetSystemTime();

    while( true )
    {
        time = chThdSleepUntilWindowed (time, time + MS2ST(500));
    }
}







