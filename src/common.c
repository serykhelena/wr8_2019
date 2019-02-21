#include <common.h>


/************************/
/*** Common ADC1 unit ***/
/************************/

#define ADC_MODE_TRIGGER                ADC_CR2_EXTEN_RISING | ADC_CR2_EXTSEL_SRC(12)
#define ADC_MODE_MANUAL                 ADC_CR2_SWSTART

#define ADC1_NUM_CHANNELS 1
#define ADC1_BUF_DEPTH    1

#define ADC3_NUM_CHANNELS 2
#define ADC3_BUF_DEPTH    2

int16_t Adc1BufSteer             = 0;
int16_t Adc3BufVoltage_9V        = 0;
int16_t Adc3BufVoltage_18V       = 0;

static adcsample_t adc1_buffer[ADC1_NUM_CHANNELS * ADC1_BUF_DEPTH];

static adcsample_t adc3_buffer[ADC3_NUM_CHANNELS * ADC3_BUF_DEPTH];

static void adc1callback ( ADCDriver *adcp, adcsample_t *buffer, size_t n );

static void adc3callback ( ADCDriver *adcp, adcsample_t *buffer, size_t n );

static const ADCConversionGroup cfg_grp1 = {
  .circular     = true,
  .num_channels = ADC1_NUM_CHANNELS,
  .end_cb       = adc1callback,
  .error_cb     = 0,
  .cr1          = ADC_RES_CONF,
  .cr2          = ADC_MODE_TRIGGER,
  .smpr1        = ADC_SMPR1_SMP_AN10(ADC_SAMPLE_144),
  .smpr2        = 0,
  .sqr1         = ADC_SQR1_NUM_CH(ADC1_NUM_CHANNELS),
  .sqr2         = 0,
  .sqr3         = ADC_SQR3_SQ1_N(ADC1_SEQ1_NUM)
};

static const ADCConversionGroup cfg_grp3 = {
  .circular     = true,
  .num_channels = ADC3_NUM_CHANNELS,
  .end_cb       = adc3callback,
  .error_cb     = 0,
  .cr1          = ADC_RES_CONF,
  .cr2          = ADC_MODE_TRIGGER,
  .smpr1        = 0,
  .smpr2        = ADC_SMPR2_SMP_AN9(ADC_SAMPLE_144) |
                  ADC_SMPR2_SMP_AN15(ADC_SAMPLE_144),
  .sqr1         = ADC_SQR1_NUM_CH(ADC3_NUM_CHANNELS),
  .sqr2         = 0,
  .sqr3         = ADC_SQR3_SQ1_N(ADC3_SEQ1_NUM) |
                  ADC_SQR3_SQ2_N(ADC3_SEQ2_NUM)
};

static const GPTConfig gpt_loop1 = {
  .frequency =  100000,
  .callback  =  NULL,
  .cr2       =  TIM_CR2_MMS_1,
  .dier      =  0U
 };

static const GPTConfig gpt_loop3 = {
  .frequency =  100000,
  .callback  =  NULL,
  .cr2       =  TIM_CR2_MMS_1,
  .dier      =  0U
 };

static ADCDriver                *globalADC1Driver    = &ADCD1;
static GPTDriver                *adc1TriggerDriver   = &GPTD4;

static ADCDriver                *globalADC3Driver    = &ADCD3;
static GPTDriver                *adc3TriggerDriver   = &GPTD6;

static void adc1callback ( ADCDriver *adcp, adcsample_t *buffer, size_t n )
{
    adcp = adcp;
    n = n;
    Adc1BufSteer = buffer[0];
    if ( buffer != adc1_buffer )
    {

    }
}

static void adc3callback ( ADCDriver *adcp, adcsample_t *buffer, size_t n )
{
    adcp = adcp;
    n = n;
    Adc3BufVoltage_9V = buffer[0];
    Adc3BufVoltage_18V = buffer[1];
    if ( buffer != adc3_buffer )
    {

    }
}

static bool     adc1_Init      = false;

static bool     adc3_Init      = false;

void InitAdc1 ( void )
{
    if ( adc1_Init )
    {
        return;
    }
    adcStart( globalADC1Driver, NULL );

    palSetLineMode( LINE_ADC123_IN10, PAL_MODE_INPUT_ANALOG );

    adcStartConversion( globalADC1Driver, &cfg_grp1, adc1_buffer, ADC1_BUF_DEPTH );

    gptStart(adc1TriggerDriver, &gpt_loop1);
    /*1 ms, triggering 1000 times per sec */
    gptStartContinuous( adc1TriggerDriver, gpt_loop1.frequency/1000 );

    adc1_Init = true;
}

void InitAdc3 ( void )
{
    if ( adc3_Init )
    {
        return;
    }
    adcStart( globalADC3Driver, NULL );

    palSetLineMode( LINE_ADC3_IN9, PAL_MODE_INPUT_ANALOG );
    palSetLineMode( LINE_ADC3_IN15, PAL_MODE_INPUT_ANALOG );

    adcStartConversion( globalADC3Driver, &cfg_grp3, adc3_buffer, ADC3_BUF_DEPTH );

    gptStart(adc3TriggerDriver, &gpt_loop3);
    /*1 ms, triggering 1000 times per sec */
    gptStartContinuous( adc3TriggerDriver, gpt_loop3.frequency/10 );

    adc3_Init = true;
}


adcsample_t GetAdcValSteer (void)
{
    return Adc1BufSteer;
}

adcsample_t GetAdcVal_STMPower (void)
{
    return Adc3BufVoltage_9V;
}

adcsample_t GetAdcVal_Battery (void)
{
    return Adc3BufVoltage_18V;
}
