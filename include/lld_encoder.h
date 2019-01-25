#ifndef INCLUDE_LLD_ENCODER_H_
#define INCLUDE_LLD_ENCODER_H_

#include <ext_adc_steering.h>
#include <chprintf.h>

/*
 * EXT driver uses input PG6
 * GPT2 used for measure time between encoder ticks
 */


/*
 * @brief   Initialize EXT, gpt and some extra 
 */
void EncoderSensInit (void);


/**
 * @ brief                             Gets wheels current speed [rpm]
 * @ param[in] speed1ImpsTicksPerMin   Number of impulses per revolution                             
 * @ return  >=0                       Current speed [rpm]
 *           -1                        Sensor is not initialized
 */
float RevPerMinSpeed (void);


/**
 * @ brief                             Gets current distance [metres]
 * @ param[in] ImpsFor1Rev             Impulse quantity per one revolution 
 * @ param[in] TotalImps               Impulse quantity from the beginning of the movement  
 * @ return  >=0                       Current distance [metres]
 *           -1                        Sensor is not initialized
 */
float GetDistance (void);


/*
 * @brief   Reset impulse quantity from the beginning of the movement 
 */
void ResetDistance (void);

/**
 * @ brief                             Gets wheels current speed [mps]                          
 * @ return  >=0                       Current speed [mps]
 *           -1                        Sensor is not initialized
 */
float GetSpeedMPS (void);


/**
 * @ brief                             Gets wheels current speed [kph]                          
 * @ return  >=0                       Current speed [kph]
 *           -1                        Sensor is not initialized
 */
float GetSpeedKPH (void);

#endif /* INCLUDE_LLD_ENCODER_H_ */