#include <tests.h>
#include <chprintf.h>
#include <lld_steering_control.h>


/**************************/
/*** CONFIGURATION ZONE ***/
/**************************/


/*
   delta for left direction:      1780  < ADC_val < 1850 ( 1790 < ADC_val < 1860)
   delta for right direction:      970  < ADC_val < 1135 ( 905  < ADC_val < 960 )
   delta for central direction:   1475  < ADC_val < 1540 ( 1510 < ADC_val < 1570)
*/


static int32_t  servLimValue        = 10; // permissible error after reading from ADC
static int32_t  servMAX             = 1820;
static int32_t  servMIN             = 1000;
static int32_t  servMID             = 1520;

static int32_t  deltaServMAX        = 300;
static int32_t  deltaServMIN        = 520;




/******************************/
/*** CONFIGURATION ZONE END ***/
/******************************/

/*** Variable configuration ***/

#define servADCchannel       ADC_SEQ1_CH

static uint8_t  filter_cnt                 = 0;
int32_t ADCfilter[] = {0,0,0,0,0};

static uint8_t  filter_cnt2                 = 0;
int32_t ADCfilter2[] = {0,0,0,0,0};

static bool     firstKalmVal                   = false;
int16_t KalmAdcVal        = 0;
int16_t lastKalmAdcVal    = 0;

/* ADC value */

static bool     isInitialized              = false;

static float leftMaxAngle                  = 34.56;
static float rightMaxAngle                 = 29.98;

static float leftFrontPosAngle             = 0;
static float rightFrontPosAngle            = 0;



void lldSteeringControlInit  (void)
{
	InitAdc();

   leftFrontPosAngle    = leftMaxAngle / deltaServMAX;
   rightFrontPosAngle   = rightMaxAngle / deltaServMIN;
    
   isInitialized = true; 
}

int16_t lldSteeringControlGetAdcVal (void)
{
    if ( !isInitialized )
	    return false;

	return GetAdcValSteer();
}


int16_t lldSteeringControlGetAdcVal_Kalman (void)
{
    if ( !isInitialized )
	    return false;

    int16_t KalmCoef          = 0.05;

    if (firstKalmVal == false)
    {
    	KalmAdcVal = GetAdcValSteer();
    	firstKalmVal = true;
    }
    else
    {
    	KalmAdcVal = KalmCoef * KalmAdcVal + lastKalmAdcVal * (1 - KalmCoef);
    }
    lastKalmAdcVal = KalmAdcVal;

	return KalmAdcVal;
}


int16_t lldSteeringControGetAdcPos_filt (void)
{
  if ( !isInitialized )
      return false;

  int16_t ADC_val = 0;
  int16_t Sum = 0;
  uint8_t i = 0;
  uint8_t j = 0;

  if (filter_cnt < 5)
  {
	  ADCfilter[filter_cnt] = GetAdcValSteer();
	  ADC_val = ADCfilter[filter_cnt];
	  filter_cnt++;
  }
  else
  {
	  while (i < 5)
	  {
		  Sum = Sum + ADCfilter[i];
		  i++;
	  }

	  while (j < 4 )
	  {
		  ADCfilter[j] = ADCfilter[j + 1];
		  j++;
	  }
	  ADCfilter[4] = GetAdcValSteer();
	  ADC_val = Sum / 5;
  }
  return ADC_val;
}

int16_t lldSteeringControGetAdcPos_doublefilt (void)
{
  if ( !isInitialized )
      return false;

  int16_t ADC_filtered_val = 0;
  int16_t Sum = 0;
  uint8_t i = 0;
  uint8_t j = 0;

  if (filter_cnt2 < 5)
  {
	  ADCfilter2[filter_cnt2] = lldSteeringControGetAdcPos_filt();
	  ADC_filtered_val = ADCfilter2[filter_cnt2];
	  filter_cnt2++;
  }
  else
  {
	  while (i < 5)
	  {
		  Sum = Sum + ADCfilter2[i];
		  i++;
	  }

	  while (j < 4 )
	  {
		  ADCfilter2[j] = ADCfilter2[j + 1];
		  j++;
	  }
	  ADCfilter2[14] = lldSteeringControGetAdcPos_filt();
	  ADC_filtered_val = Sum / 5;
  }
  return ADC_filtered_val;
}



steer_angle_t lldSteeringControlGetAngle (void)
{
  if ( !isInitialized )
      return false;
    
  int16_t RotateAngle  = 0;
  int16_t lldAdcVal = GetAdcValSteer();
     
    if (lldAdcVal < servMIN - servLimValue)
  {
      lldAdcVal = servMIN - servLimValue ;  
  }
    else if (lldAdcVal > servMAX + servLimValue)
  {
      lldAdcVal = servMAX + servLimValue; 
  }
  if (lldAdcVal - servMID > 0)
  {
      RotateAngle = (lldAdcVal - servMID) * leftFrontPosAngle;
  }
  else
  {
      RotateAngle = (lldAdcVal - servMID) * rightFrontPosAngle;
  }
  return RotateAngle;  
}

