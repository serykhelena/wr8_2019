#ifndef INCLUDE_EXT_ADC_STEERING_H_
#define INCLUDE_EXT_ADC_STEERING_H_

#include <hal.h>
#include <ch.h>


void ExtDriverInit ( void );


#define ADC_RES_CONF                ADC_CR1_12B_RESOLUTION

/*Steer sensor position input */
#define ADC_SEQ1_NUM                ADC_CHANNEL_IN10
#define ADC_SEQ1_LINE               PAL_LINE( GPIOC, 0 )
#define ADC_SEQ1_CH                 0

/**
 * @brief       Preparing to start ADC operations  
*/
void InitAdc ( void );

/**
 * @brief       Get value of ADC channel
 * @return      Value of ADC sampling
*/
adcsample_t FindADC1Val ();



/* Additional ADC constants */
#define ADC_CR1_12B_RESOLUTION      (0)
#define ADC_CR1_10B_RESOLUTION      (ADC_CR1_RES_0)
#define ADC_CR1_8B_RESOLUTION       (ADC_CR1_RES_1)
#define ADC_CR1_6B_RESOLUTION       (ADC_CR1_RES_0 | ADC_CR1_RES_1)

#endif /* INCLUDE_EXT_ADC_STEERING_H_ */