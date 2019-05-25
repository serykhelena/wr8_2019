#include <common.h>
#include <tests.h>
#include <odometry.h>
#include <lld_encoder.h>


#define VTPEDIOD	10
#define LPFCOEF		(float)0.1

static virtual_timer_t odom_vt;

float OdomDistance                  = 0;
float OdomPrevDist                  = 0;
float OdomRawSpeed 	                = 0;

static float   GearRatio            = 0.105;
static float   circumference        = 25.13; // 2 * pi * WheelRadius [cm]


static bool     firstOdomLPFSpeed   = false;
float           OdomLPFSpeed      	= 0;
float           lastOdomLPFSpeed   	= 0;

static bool IsInit = false;

static void odom_cb(void *arg) {

    arg = arg; // to avoid warnings

    float RevQuantity = 0;

    RevQuantity = lldEncoderGetRevolutions();
     /* distance for 1 revolution is wheel circumference in cm*/
     /*  total distance is N revolutions( N wheel circumferences ) */
     /* [S = 2 * pi * (distance for 1 revolution)] */
    OdomDistance = circumference * RevQuantity * GearRatio;

    OdomRawSpeed = (OdomDistance - OdomPrevDist) * 100; // ds/dt * 100 [cm/10 ms -> cm/sec]

    if (firstOdomLPFSpeed == false)
    {
    	OdomLPFSpeed = OdomRawSpeed;
    	firstOdomLPFSpeed = true;
    }
    else
    {
    	OdomLPFSpeed = LPFCOEF * OdomRawSpeed + lastOdomLPFSpeed * ((float)1 - LPFCOEF);
    }
    lastOdomLPFSpeed = OdomLPFSpeed;

    OdomPrevDist = OdomDistance;

	chSysLockFromISR();
	chVTSetI(&odom_vt, MS2ST(VTPEDIOD), odom_cb, NULL);
	chSysUnlockFromISR();
}

void OdometryInit(void)
{
	if (IsInit)
		return;
	chVTObjectInit(&odom_vt);
    lldEncoderSensInit();
    chVTSet(&odom_vt, MS2ST(VTPEDIOD), odom_cb, NULL);
    IsInit = true;
}


int16_t OdometryGetSpeedRPM (void)
{
    drvctrl_t RPM = 0;

    RPM = lldEncoderGetSpeedRPM() * GearRatio;

    return RPM;
}

/**
 * TODO COMMENTS NEED TO FIX
 */
float OdometryGetSpeedMPS (void)
{
    float MPS = 0;

    MPS = lldEncoderGetSpeedMPS() * GearRatio;

    return MPS;
}

float OdometryGetSpeedSmPS (void)
{
    return OdomRawSpeed;
}


float OdometryGetLPFSpeedSmPS (void)
{
	return OdomLPFSpeed;
}


int16_t OdometryGetSpeedKPH (void)
{
    drvctrl_t KPH = 0;

    KPH = lldEncoderGetSpeedKPH() * GearRatio;

    return KPH;
}


float OdometryGetDistance (void)
{
     return OdomDistance;
}

void OdometryResetDistance (void)
{
    /* Total distance is determined by all encoder ticks */
	lldEncoderResetDistance();
}
