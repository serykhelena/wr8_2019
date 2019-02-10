#ifndef INCLUDE_COMMON_H_
#define INCLUDE_COMMON_H_

#include <ch.h>
#include <hal.h>

#include <stdint.h>
#include <stdlib.h>

#define     PROGRAM_ROUTINE_MASTER                      0
#define     PROGRAM_ROUTINE_TEST_STEERING_CONTROL       1
#define     PROGRAM_ROUTINE_TEST_CHARGE_LEVEL           2


#define     MAIN_PROGRAM_ROUTINE                        PROGRAM_ROUTINE_TEST_CHARGE_LEVEL


#define ADC_RES_CONF                ADC_CR1_12B_RESOLUTION

/*Steer sensor position input */
#define ADC_SEQ1_NUM                ADC_CHANNEL_IN10
#define ADC_SEQ1_LINE               PAL_LINE( GPIOC, 0 )
#define ADC_SEQ1_CH                 0

/*Charge level */
#define ADC_SEQ2_NUM                ADC_CHANNEL_IN10
#define ADC_SEQ2_LINE               PAL_LINE( GPIOC, 3 )
#define ADC_SEQ2_CH                 1

/**
 * @brief       Preparing to start ADC operations
*/
void InitAdc ( void );

/**
 * @brief       Get value of ADC channel 0
 * @return      Value of ADC sampling
*/
adcsample_t GetAdcValSteer (void);

/**
 * @brief       Get value of ADC channel 1
 * @return      Value of ADC sampling
*/
adcsample_t GetAdcValVoltage (void);



/* Additional ADC constants */
#define ADC_CR1_12B_RESOLUTION      (0)
#define ADC_CR1_10B_RESOLUTION      (ADC_CR1_RES_0)
#define ADC_CR1_8B_RESOLUTION       (ADC_CR1_RES_1)
#define ADC_CR1_6B_RESOLUTION       (ADC_CR1_RES_0 | ADC_CR1_RES_1)





#endif /* INCLUDE_COMMON_H_ */
