#include <tests.h>
#include <lld_wheel_rotate.h>

//#include <common.h>


/**************************/
/*** CONFIGURATION ZONE ***/
/**************************/


/*
   delta for left direction:      1780  < ADC_val < 1850
   delta for right direction:     1090  < ADC_val < 1150
   delta for central direction:   1540  < ADC_val < 1580
*/


static int32_t  servLimValue        = 10; // permissible error after reading from ADC
static int32_t  servMAX             = 1800;
static int32_t  servMIN             = 1120; 
static int32_t  servMID             = 1560;

static int32_t  deltaServMAX        = 255; 
static int32_t  deltaServMIN        = 440;




/******************************/
/*** CONFIGURATION ZONE END ***/
/******************************/

/*** Variable configuration ***/

#define servADCchannel       ADC_SEQ1_CH


/* ADC value */

static bool     isInitialized               = false;

static float leftFrontPosVal               = 0;
static float rightFrontPosVal              = 0;

static float leftMaxAngle                  = 34.56;
static float rightMaxAngle                 = 29.98;

static float leftFrontPosAngle             = 0;
static float rightFrontPosAngle            = 0;


void lldServInit (void)
{
	AdcInit();
    
    /*wheels position from -100 to 100 */
   leftFrontPosVal    = 100.0f / deltaServMAX;
   rightFrontPosVal   = 100.0f / deltaServMIN;
    
   leftFrontPosAngle    = leftMaxAngle / deltaServMAX;
   rightFrontPosAngle   = rightMaxAngle / deltaServMIN;
    
   isInitialized = true; 
}

int16_t lldGetFrontWheelAdcPos (void)
{
  if ( !isInitialized )
      return false;
  
  return FindADC1Val();
}

int16_t lldGetFrontWheelVal (void)
{
  if ( !isInitialized )
      return false;
    
  int16_t RotateVal  = 0;
  int16_t lldAdcVal = FindADC1Val();
     
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
      RotateVal = (lldAdcVal - servMID) * leftFrontPosVal;
  }
  else
  {
      RotateVal = (lldAdcVal - servMID) * rightFrontPosVal;
  }
  return RotateVal;  
}

int16_t lldGetFrontWheelAngle (void)
{
  if ( !isInitialized )
      return false;
    
  int16_t RotateAngle  = 0;
  int16_t lldAdcVal = FindADC1Val();
     
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

