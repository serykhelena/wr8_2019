#include <tests.h>
#include <lld_steering_control.h>


int32_t AdcVal = 0;
int16_t PosVal = 0;
int16_t Angle  = 0;


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


void testSteeringControl (void)
{
   lldSteeringControlInit();
   /* Start working GPT driver in asynchronous mode */
   gptStart(Tim5, &gpt5cfg);
   gptStartContinuous(Tim5, period_50ms);
    while( true )
    {
    	last_periodCheckPoint = gptGetCounterX(Tim5);
    	//AdcVal = lldSteeringControlGetAdcVal();
    	AdcVal = lldSteeringControlGetAdcVal_Kalman ();
    	periodCheckPoint = gptGetCounterX(Tim5);
    	KalmanTime = total_ticks + periodCheckPoint - last_periodCheckPoint;
    	total_ticks = 0;
        //sdWrite( &SD7, (uint16_t *)&AdcVal, sizeof( AdcVal ) );
        chprintf( (BaseSequentialStream *)&SD7, "%d \n", AdcVal );
        chThdSleepMilliseconds( 200 );
    }
}



