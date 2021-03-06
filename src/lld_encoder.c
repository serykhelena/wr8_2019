#include <tests.h>
#include <lld_encoder.h>
#include <math.h>

/**************************/
/*** CONFIGURATION ZONE ***/
/**************************/

#define WheelRadius     0.04
#define ScanFreq        5

static float circumference       = 0.2513; // 2 * pi * WheelRadius
static float OverflowsInTimeDiap = 1000;


/******************************/
/*** CONFIGURATION ZONE END ***/
/******************************/

#define encoderChA       PAL_LINE ( GPIOD, 4 )
#define encoderChB       PAL_LINE ( GPIOD, 5 )

static void extcbA ( EXTDriver *extp, expchannel_t channel );
static void extcbB ( EXTDriver *extp, expchannel_t channel );

static void gpt_callback ( GPTDriver *Tim2 );
static GPTDriver                     *Tim2 = &GPTD7;

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
int32_t EncImps               = 0;
int32_t timeSum                 = 0;
int8_t  ScanCnt                 = 0;
bool    SpeedState              = 0;
int32_t timeForSpeed            = 0;

static int32_t   maxOverflows   = 0;

static bool    isInitialized            = false;
static bool    drivingWheelsMoving      = false;

uint8_t ExtAcnt                         = 0;
uint8_t ExtBcnt                         = 0;
uint8_t ExtAtick                        = 0;
uint8_t ExtBtick                        = 0;
int8_t Direction                        = 0;
int32_t IntRevolutions                     = 0;


#define IMPSFOR1REV         500

static float speed1ImpsTicksPerMin = 0;

EXTConfig extcfg = {
.channels =
    {
        [0]  = {EXT_CH_MODE_DISABLED, NULL},
        [1]  = {EXT_CH_MODE_DISABLED, NULL},
        [2]  = {EXT_CH_MODE_DISABLED, NULL},
        [3]  = {EXT_CH_MODE_DISABLED, NULL},
        [4]  = {EXT_CH_MODE_FALLING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOD, extcbA},// PD4 green
        [5]  = {EXT_CH_MODE_FALLING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOD, extcbB},// PD5 white
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

/*
 * @brief   Initialize EXT, gpt  
 */
void lldEncoderSensInit (void)
{
    if ( isInitialized )
        return;
    /* Start working EXT driver */
    extStart( &EXTD1, &extcfg );
    /*Set up EXT channels A and B hardware pin mode as digital inputs*/
    palSetLineMode( encoderChA, PAL_MODE_INPUT_PULLUP );
    palSetLineMode( encoderChB, PAL_MODE_INPUT_PULLUP );
    
    /* Start working GPT driver in asynchronous mode */
    gptStart(Tim2, &gpt2cfg);
    gptStartContinuous(Tim2, period_50ms);
    
    total_ticks = 0;
    
    int32_t Period_ticks = 1000.0 * period_50ms / gpt2cfg.frequency;
  
    speed1ImpsTicksPerMin = 60 * gpt2cfg.frequency / IMPSFOR1REV;

    maxOverflows = OverflowsInTimeDiap / Period_ticks;
  
    max_ticks = period_50ms * maxOverflows;
    
    isInitialized       = true;
}

/* Timer 2 callback function */
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

    ExtAtick = 1;

    if (palReadPad(GPIOD, 5))
    {
    	Direction = -1;
    	EncImps --;
    }
    else
    {
    	Direction = 1;
    	EncImps ++;
    }

    if (EncImps >= 500)
    {
    	EncImps = 0;
    	IntRevolutions++;
    	    }
    else if (EncImps <= -500)
    	{
    	    EncImps = 0;
			IntRevolutions--;
    	}

    /*time between ticks*/
    FromTickToTickEncoder = 0;
    /* number of ticks from last overflow */
    periodCheckPoint = gptGetCounterX(Tim2);
	if ( drivingWheelsMoving )
	{
		FromTickToTickEncoder = total_ticks + periodCheckPoint - last_periodCheckPoint;
	}
    total_ticks = 0;
    last_periodCheckPoint = periodCheckPoint;
    ScanCnt++;

    if (ScanCnt <= ScanFreq)
    {
        timeSum += FromTickToTickEncoder;
    }
    else
    {
    	SpeedState = true;
    	timeForSpeed = timeSum / ScanFreq;
        timeSum  = 0;
        ScanCnt  = 1;
        timeSum += FromTickToTickEncoder;
    }

    drivingWheelsMoving = true;

    ExtAtick = 0;
    ExtBtick = 0;
}


/* EXT channel B (input A2) */
static void extcbB(EXTDriver *extp, expchannel_t channel)
{
    extp = extp; channel = channel;

}



/**
 * @ brief                             Gets current quantity of revolutions
 * @ return  >=0                       Current quantity of revolutions (in 1 revolution - 100 encoder ticks)
 *           -1                        Sensor is not initialized
 */
float lldEncoderGetRevolutions(void)
{
    if ( isInitialized == false )
    {
	    return -1;
    }
    float Revs = 0;
    /*calculates the number of revolutions - ratio of total ticks of the encoder to ticks per revolution*/
    Revs = IntRevolutions + (float)EncImps / IMPSFOR1REV;
    return Revs ;
}

/**
 * @ brief                             Gets current total quantity of encoder ticks
 * @ return  >=0                       Current total quantity of encoder ticks
 *           -1                        Sensor is not initialized
 */
int32_t lldEncoderGetEncTicks(void)
{
    if ( isInitialized == false )
    {
	    return -1;
    }
    /*total ticks of the encoder from begining of movement*/
    return EncImps ;
}

/**
 * @ brief                             Gets wheels current speed [rpm]                           
 * @ return  >=0                       Current speed [rpm]
 *           -1                        Sensor is not initialized
 */
encValue_t lldEncoderGetSpeedRPM (void)
{
    if ( isInitialized == false )
    {
        return -1;
    }

    if ( drivingWheelsMoving == false )
    {
        return 0;
    }
    /* Speed in revolutions per minute */
    encValue_t RevSpeed = 0;
 
    if (FromTickToTickEncoder == 0)
    {
	    RevSpeed  = 0 ;
    }
    else
    {
    	if (SpeedState)
    	{
    	    RevSpeed  = Direction * speed1ImpsTicksPerMin / timeForSpeed;
    	    SpeedState = false;
    	}
    }
    return RevSpeed;
}



/*
 * @brief   Reset impulse quantity from the beginning of the movement 
 */
void lldEncoderResetDistance (void)
{
    /* Total distance is determined by all encoder ticks */
    EncImps = 0;
}

/**
 * @ brief                             Gets wheels current speed [mps]                          
 * @ return  >=0                       Current speed [mps]
 *           -1                        Sensor is not initialized
 */
encValue_t lldEncoderGetSpeedMPS (void)
{
    if ( !isInitialized )
    {
        return -1;
    }

    float SpeedMPS = 0;
    /* Get speed in revolutions per second */    
    float speedRevPerSec = lldEncoderGetSpeedRPM () * 0.0167 ;
    /* [V = 2 * pi * (speed in revolutions per second)] */
    SpeedMPS = circumference * speedRevPerSec ;
        
    return SpeedMPS;
}

/**
 * @ brief                             Gets wheels current speed [kph]                     
 * @ return  >=0                       Current speed [kph]
 *           -1                        Sensor is not initialized
 */
encValue_t lldEncoderGetSpeedKPH (void)
{
    if ( !isInitialized )
    {
        return -1;
    }
    float SpeedKPH = 0;
    /* 1 meter per second = 3.6 kilometres per hour */
    /* V [kph] = 3.6 * V [mps] */
    SpeedKPH = 3.6 * lldEncoderGetSpeedMPS();
        
    return SpeedKPH;
}
