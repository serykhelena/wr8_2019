#include <tests.h>
#include <lld_steering_control.h>



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

static uint8_t  filter_cnt                 = 0;
int32_t ADCfilter[] = {0,0,0,0,0};
static uint8_t  filter_cnt2                 = 0;
int32_t ADCfilter2[] = {0,0,0,0,0};

/* ADC value */

static bool     isInitialized              = false;

static float leftFrontPosVal               = 0;
static float rightFrontPosVal              = 0;

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
	  ADCfilter[filter_cnt] = FindADC1Val();
	  ADC_val = ADCfilter[filter_cnt];
	  filter_cnt++;
  }
  else
  {
	  //filter_cnt = 4;
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
	  ADCfilter[4] = FindADC1Val();
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
	  ADCfilter2[filter_cnt2] = GetFrontWheelAdcPos_filt();
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
	  ADCfilter2[4] = GetFrontWheelAdcPos_filt();
	  ADC_filtered_val = Sum / 5;
  }
  return ADC_filtered_val;
}



int16_t lldSteeringControlGeAngle (void)
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

