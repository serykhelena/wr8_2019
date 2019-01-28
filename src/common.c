#include <common.h>

///////////////////////EXT init///////////////////////////

static bool extDrInit = false;


void ExtDriverInit ( void )
{
    if ( extDrInit )
        return;
    static const EXTConfig extcfg = {
  .channels =
  {
    [0]  = {EXT_CH_MODE_DISABLED, NULL},
    [1]  = {EXT_CH_MODE_DISABLED, NULL},
    [2]  = {EXT_CH_MODE_DISABLED, NULL},
    [3]  = {EXT_CH_MODE_DISABLED, NULL},
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

    extDrInit = true;
}

///////////////////////ADC1 init///////////////////////////

#define ADC_MODE_TRIGGER                ADC_CR2_EXTEN_RISING | ADC_CR2_EXTSEL_SRC(12)
#define ADC_MODE_MANUAL                 ADC_CR2_SWSTART

#define ADC1_NUM_CHANNELS 1
#define ADC1_BUF_DEPTH    1

int16_t AdcBuf        = 0;

static adcsample_t adc_buffer[ADC1_NUM_CHANNELS * ADC1_BUF_DEPTH];

static void adccallback ( ADCDriver *adcp, adcsample_t *buffer, size_t n );

static const ADCConversionGroup cfg_grp1 = {
  .circular     = true,
  .num_channels = ADC1_NUM_CHANNELS,
  .end_cb       = adccallback,
  .error_cb     = 0,
  .cr1          = ADC_RES_CONF,
  .cr2          = ADC_MODE_TRIGGER,
  .smpr1        = ADC_SMPR1_SMP_AN10(ADC_SAMPLE_144),
  .smpr2        = 0,
  .sqr1         = ADC_SQR1_NUM_CH(ADC1_NUM_CHANNELS),
  .sqr2         = 0,
  .sqr3         = ADC_SQR3_SQ1_N(ADC_SEQ1_NUM)
};

static const GPTConfig gpt_loop = {
  .frequency =  100000,
  .callback  =  NULL,
  .cr2       =  TIM_CR2_MMS_1,
  .dier      =  0U
 };

static ADCDriver                *globalADCDriver    = &ADCD1;
static GPTDriver                *adcTriggerDriver   = &GPTD4;

static void adccallback ( ADCDriver *adcp, adcsample_t *buffer, size_t n )
{
    adcp = adcp;
    n = n;
    AdcBuf = buffer[0];
    if ( buffer != adc_buffer )
    {

    }
}

static bool     adc_Init      = false;

void InitAdc ( void )
{
    if ( adc_Init )
    {
        return;
    }
    adcStart( globalADCDriver, NULL );

    palSetLineMode( LINE_ADC123_IN10, PAL_MODE_INPUT_ANALOG );

    adcStartConversion( globalADCDriver, &cfg_grp1, adc_buffer, ADC1_BUF_DEPTH );

    gptStart(adcTriggerDriver, &gpt_loop);
    /*1 ms, triggering 1000 times per sec */
    gptStartContinuous( adcTriggerDriver, gpt_loop.frequency/1000 );

    adc_Init = true;
}


adcsample_t GetAdcVal (void)
{
    return AdcBuf;
}

