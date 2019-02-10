#include <common.h>


/************************/
/*** Common ADC1 unit ***/
/************************/

#define ADC_MODE_TRIGGER                ADC_CR2_EXTEN_RISING | ADC_CR2_EXTSEL_SRC(12)
#define ADC_MODE_MANUAL                 ADC_CR2_SWSTART

#define ADC1_NUM_CHANNELS 2
#define ADC1_BUF_DEPTH    2

int16_t AdcBufSteer          = 0;
int16_t AdcBufVoltage        = 0;

static adcsample_t adc_buffer[ADC1_NUM_CHANNELS * ADC1_BUF_DEPTH];

static void adccallback ( ADCDriver *adcp, adcsample_t *buffer, size_t n );

static const ADCConversionGroup cfg_grp1 = {
  .circular     = true,
  .num_channels = ADC1_NUM_CHANNELS,
  .end_cb       = adccallback,
  .error_cb     = 0,
  .cr1          = ADC_RES_CONF,
  .cr2          = ADC_MODE_TRIGGER,
  .smpr1        = ADC_SMPR1_SMP_AN10(ADC_SAMPLE_144) |
                  ADC_SMPR1_SMP_AN13(ADC_SAMPLE_144),
  .smpr2        = 0,
  .sqr1         = ADC_SQR1_NUM_CH(ADC1_NUM_CHANNELS),
  .sqr2         = 0,
  .sqr3         = ADC_SQR3_SQ1_N(ADC_SEQ1_NUM) |
                  ADC_SQR3_SQ2_N(ADC_SEQ2_NUM)
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
    AdcBufSteer = buffer[0];
    AdcBufVoltage = buffer[1];
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
    palSetLineMode( LINE_ADC123_IN13, PAL_MODE_INPUT_ANALOG );

    adcStartConversion( globalADCDriver, &cfg_grp1, adc_buffer, ADC1_BUF_DEPTH );

    gptStart(adcTriggerDriver, &gpt_loop);
    /*1 ms, triggering 1000 times per sec */
    gptStartContinuous( adcTriggerDriver, gpt_loop.frequency/1000 );

    adc_Init = true;
}


adcsample_t GetAdcValSteer (void)
{
    return AdcBufSteer;
}

adcsample_t GetAdcValVoltage (void)
{
    return AdcBufVoltage;
}


