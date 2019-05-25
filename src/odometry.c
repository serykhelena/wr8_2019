#include <common.h>
#include <tests.h>
#include <odometry.h>
#include <lld_encoder.h>


static float   GearRatio         = 0.105;
static float   circumference     = 0.2513; // 2 * pi * WheelRadius

void OdometryInit(void)
{

    lldEncoderSensInit();
}


int16_t OdometryGetSpeedRPM (void)
{
    drvctrl_t RPM = 0;

    RPM = lldEncoderGetSpeedRPM() * GearRatio;

    return RPM;
}

float OdometryGetSpeedMPS (void)
{
    float MPS = 0;

    MPS = lldEncoderGetSpeedMPS() * GearRatio;

    return MPS;
}

int16_t OdometryGetSpeedKPH (void)
{
    drvctrl_t KPH = 0;

    KPH = lldEncoderGetSpeedKPH() * GearRatio;

    return KPH;
}

int16_t OdometryGetDistance (void)
{
    drvctrl_t distance    = 0;
    drvctrl_t RevQuantity = 0;

    RevQuantity = lldEncoderGetRevolutions();
     /* distance for 1 revolution is wheel circumference in cm*/
     /*  total distance is N revolutions( N wheel circumferences ) */
     /* [S = 2 * pi * (distance for 1 revolution)] */
    distance = circumference * RevQuantity * 100;

     return distance;
}

void OdometryResetDistance (void)
{
    /* Total distance is determined by all encoder ticks */
	lldEncoderResetDistance();
}
