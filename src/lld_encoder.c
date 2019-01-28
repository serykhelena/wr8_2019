#include <tests.h>
#include <lld_encoder.h>

/**************************/
/*** CONFIGURATION ZONE ***/
/**************************/

static float WheelRadius              = 0.08;
static float OverflowsInTimeDiap      = 1000;

/******************************/
/*** CONFIGURATION ZONE END ***/
/******************************/

#define encoderChA       PAL_LINE ( GPIOC, 0 )
#define encoderChB       PAL_LINE ( GPIOC, 3 )

static void extcbA ( EXTDriver *extp, expchannel_t channel );
static void extcbB ( EXTDriver *extp, expchannel_t channel );

static void gpt_callback ( GPTDriver *Tim2 );
static GPTDriver                     *Tim2 = &GPTD2;

static const GPTConfig gpt2cfg = { 
  .frequency =  100000,
  .callback  =  gpt_callback,
  .cr2       =  0,
  .dier      =  0U 
};

#define period_20ms         gpt2cfg.frequency/50
#define period_100ms        gpt2cfg.frequency/10
#define period_50ms         gpt2cfg.frequency/20


int32_t total_ticks             = 0;
int32_t max_ticks               = 0;
int32_t periodCheckPoint        = 0;
int32_t last_periodCheckPoint   = 0;
int32_t FromTickToTickEncoder   = 0;
int32_t TotalImps               = 0;

static int32_t   maxOverflows   = 0;

static bool    isInitialized            = false;
static bool    drivingWheelsMoving      = false;

int8_t ExtAcnt                  = 0;
int8_t ExtBcnt                  = 0;

#define ImpsFor1Rev         100

static float speed1ImpsTicksPerMin = 0;

void EncoderSensInit (void)
{
    if ( isInitialized )
        return;
        
    EXTConfig extcfg = {
    .channels =
     {
      [0]  = {EXT_CH_MODE_FALLING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOC, extcbA}, // PC0
      [1]  = {EXT_CH_MODE_DISABLED, NULL},
      [2]  = {EXT_CH_MODE_DISABLED, NULL},
      [3]  = {EXT_CH_MODE_FALLING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOC, extcbB}, // PC3
      [4]  = {EXT_CH_MODE_DISABLED, NULL},
      [5]  = {EXT_CH_MODE_DISABLED, NULL},
      [6]  = {EXT_CH_MODE_DISABLED, NULL},
      [7]  = {EXT_CH_MODE_DISABLED, NULL},
      [8]  = {EXT_CH_MODE_DISABLED, NULL},
      [9]  = {EXT_CH_MODE_DISABLED, NULL},
      [10] = {EXT_CH_MODE_DISABLED, NULL},
      [11] = {EXT_CH_MODE_DISABLED, NULL},
      [12] = {EXT_CH_MODE_DISABLED, NULL},
      [13] = {EXT_CH_MODE_DISABLED, NULL}, 
      [14] = {EXT_CH_MODE_DISABLED, NULL},
      [15] = {EXT_CH_MODE_DISABLED, NULL},
    }
  };
  extStart( &EXTD1, &extcfg );  

  palSetLineMode( encoderChA, PAL_MODE_INPUT_PULLUP );
  palSetLineMode( encoderChB, PAL_MODE_INPUT_PULLUP );
  
  gptStart(Tim2, &gpt2cfg);
  gptStartContinuous(Tim2, period_50ms); 
    
  total_ticks = 0;
    
  int32_t Period_ticks = 1000.0 * period_50ms / gpt2cfg.frequency;    
  
  speed1ImpsTicksPerMin = 60 * gpt2cfg.frequency / ImpsFor1Rev;

  maxOverflows = OverflowsInTimeDiap / Period_ticks;
  
  max_ticks = period_50ms * maxOverflows;
    
  isInitialized       = true;
}

static void gpt_callback (GPTDriver *gptd)
{
    gptd = gptd;
    
    total_ticks += period_50ms;
    
    if ( total_ticks >= max_ticks )
    {
        drivingWheelsMoving = false;
    }
}



static void extcbA(EXTDriver *extp, expchannel_t channel)
{
    extp = extp; channel = channel;

    ExtAcnt = 1;
}

static void extcbB(EXTDriver *extp, expchannel_t channel)
{
    extp = extp; channel = channel;

    ExtBcnt = 1;

    if (ExtAcnt == 1 && ExtBcnt == 1)
    {
    	FromTickToTickEncoder = 0;
    	periodCheckPoint = gptGetCounterX(Tim2);

    	if ( drivingWheelsMoving )
    	{
    	    FromTickToTickEncoder = total_ticks + periodCheckPoint - last_periodCheckPoint;
    	    TotalImps++;
    	}
    	total_ticks = 0;
    	last_periodCheckPoint = periodCheckPoint;

    	drivingWheelsMoving = true;

    	ExtAcnt = 0;
    	ExtBcnt = 0;
    }
}

uint16_t GetRevolutions(void)
{
    if ( isInitialized == false )
	{
	    return -1;
	}

	return TotalImps / ImpsFor1Rev ;
}


uint32_t GetEncTicks(void)
{
	if ( isInitialized == false )
	{
	    return -1;
	}

	return TotalImps ;
}


float GetSpeedRPM (void)
{
  if ( isInitialized == false )
  {
      return -1;
  }

  if ( drivingWheelsMoving == false )
  {
      return 0; 
  }

  float RevSpeed = 0;   
 
  if (FromTickToTickEncoder == 0)
  {
	  RevSpeed  = 0 ;
  }
  else
  {
	  RevSpeed  = speed1ImpsTicksPerMin / FromTickToTickEncoder   ;
  }
  return RevSpeed;
}


float GetDistance (void)
{
    if ( !isInitialized )
    {
      return -1;
    }
    float RevQuantity = 0;
    float distance = 0;

    RevQuantity = TotalImps / ImpsFor1Rev;
    distance = 6.28 * RevQuantity * WheelRadius;

    return distance;
}

void ResetDistance (void)
{
    TotalImps = 0;
}

float GetSpeedMPS (void)
{
    if ( !isInitialized )
    {
      return -1;
    }

    float SpeedMPS = 0;
    float speedRevPerSec = GetSpeedRPM () / 60.0f ;
    
    SpeedMPS = 6.28 * WheelRadius * speedRevPerSec;
        
    return SpeedMPS;
}

float GetSpeedKPH (void)
{
    if ( !isInitialized )
    {
      return -1;
    }
    float SpeedKPH = 0;
    
    SpeedKPH = 3.6 * GetSpeedMPS(); 
        
    return SpeedKPH;
}
