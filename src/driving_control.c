#include <common.h>
#include <tests.h>
#include <driving_control.h>
#include <lld_encoder.h>


static float   GearRatio         = 0.105;
static float   circumference     = 0.2513; // 2 * pi * WheelRadius

static bool     firstLPval               = false;
float LPAdcVal                           = 0;
float lastLPAdcVal                       = 0;

void DrivingControlInit(void)
{

    lldEncoderSensInit();
}


int16_t DrivingControlGetSpeedRPM (void)
{
    drvctrl_t RPM = 0;

    RPM = lldEncoderGetSpeedRPM() * GearRatio;

    return RPM;
}

float DrivingControlGetSpeedMPS (void)
{
    float MPS = 0;

    MPS = lldEncoderGetSpeedMPS() * GearRatio;

    return MPS;
}

float DrivingControlGetSpeedMPS_lowpass (void)
{
    float LPcoef          = 0.1;


    if (firstLPval == false)
    {
    	LPAdcVal = DrivingControlGetSpeedMPS();
    	firstLPval = true;
    }
    else
    {
    	LPAdcVal = LPcoef * DrivingControlGetSpeedMPS() + lastLPAdcVal * ((float)1 - LPcoef);
    }
    lastLPAdcVal = LPAdcVal;

	return LPAdcVal;
}

int16_t DrivingControlGetSpeedKPH (void)
{
    drvctrl_t KPH = 0;

    KPH = lldEncoderGetSpeedKPH() * GearRatio;

    return KPH;
}

int16_t DrivingControlGetDistance (void)
{
    drvctrl_t distance    = 0;
    drvctrl_t RevQuantity = 0;

    RevQuantity = lldEncoderGetRevolutions();
     /* distance for 1 revolution is wheel circumference */
     /*  total distance is N revolutions( N wheel circumferences ) */
     /* [S = 2 * pi * (distance for 1 revolution)] */
    distance = circumference * RevQuantity;

     return distance;
}

void DrivingControlResetDistance (void)
{
    /* Total distance is determined by all encoder ticks */
	lldEncoderResetDistance();
}
