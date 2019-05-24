#include <common.h>
#include <tests.h>
#include <odometry.h>
#include <lld_encoder.h>


static float   GearRatio         = 0.105;
static float   circumference     = 0.2513; // 2 * pi * WheelRadius

static bool    firstLPval        = false;
float          LPAdcVal          = 0;
float          lastLPAdcVal      = 0;

static bool    isInitialized            = false;

/*
 * @brief   Initialize Odometry unit
 */
void odometryInit(void)
{
    if ( isInitialized )
            return;

    lldEncoderSensInit();
    isInitialized       = true;

}

/**
 * TODO COMMENTS
 */
int16_t odometryGetSpeedRPM (void)
{
    odomValue_t RPM = 0;

    RPM = lldEncoderGetSpeedRPM() * GearRatio;

    return RPM;
}

/**
 * TODO COMMENTS
 */
float odometryGetSpeedMPS (void)
{
    float MPS = 0;

    MPS = lldEncoderGetSpeedMPS() * GearRatio;

    return MPS;
}

/**
 * TODO COMMENTS
 */
float odometryGetLPFSpeedMPS (void)
{
    float LPcoef          = 0.1;

    if (firstLPval == false)
    {
    	LPAdcVal = odometryGetSpeedMPS();
    	firstLPval = true;
    }
    else
    {
    	LPAdcVal = LPcoef * odometryGetSpeedMPS() + lastLPAdcVal * ((float)1 - LPcoef);
    }
    lastLPAdcVal = LPAdcVal;

	return LPAdcVal;
}

/**
 * TODO COMMENTS
 */
int16_t odometryGetSpeedKPH (void)
{
    odomValue_t KPH = 0;

    KPH = lldEncoderGetSpeedKPH() * GearRatio;

    return KPH;
}

/**
 * @ brief      Gets current distance [m]
 * @ return     Current distance [m]
 */
int16_t odometryGetDistance (void)
{
    odomValue_t distance    = 0;
    odomValue_t RevQuantity = 0;

    RevQuantity = lldEncoderGetRevolutions();
     /* distance for 1 revolution is wheel circumference */
     /*  total distance is N revolutions( N wheel circumferences ) */
     /* [S = 2 * pi * (distance for 1 revolution)] */
    distance = circumference * RevQuantity;

     return distance;
}


/*
 * @brief   Reset impulse quantity from
 *          the beginning of the movement
 */
void odometryResetDistance (void)
{
    /* Total distance is determined by all encoder ticks */
	lldEncoderResetDistance();
}
