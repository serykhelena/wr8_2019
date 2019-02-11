#ifndef INCLUDE_LLD_ENCODER_H_
#define INCLUDE_LLD_ENCODER_H_

#include <chprintf.h>

/*
 * EXT driver uses input PС0 (A1) - channel A and PC3 (A2) - channel B
 * GPT2 used for measure time between encoder ticks
 */


/*
 * @brief   Initialize EXT, gpt  
 */
void lldEncoderSensInit (void);


/**
 * @ brief                             Gets current quantity of revolutions
 * @ return  >=0                       Current quantity of revolutions (in 1 revolution - 100 encoder ticks)
 *           -1                        Sensor is not initialized
 */
uint16_t lldEncoderGetRevolutions(void);


/**
 * @ brief                             Gets current total quantity of encoder ticks
 * @ return  >=0                       Current total quantity of encoder ticks
 *           -1                        Sensor is not initialized
 */
uint32_t lldEncoderGetEncTicks(void);


/**
 * @ brief                             Gets wheels current speed [rpm]
 * @ param[in] speed1ImpsTicksPerMin   Number of impulses per revolution                             
 * @ return  >=0                       Current speed [rpm]
 *           -1                        Sensor is not initialized
 */
float lldEncoderGetSpeedRPM (void);



/**
 * @ brief                             Gets current distance [metres]
 * @ param[in] ImpsFor1Rev             Impulse quantity per one revolution 
 * @ param[in] TotalImps               Impulse quantity from the beginning of the movement  
 * @ return  >=0                       Current distance [metres]
 *           -1                        Sensor is not initialized
 */
float lldEncoderGetDistance (void);


/*
 * @brief   Reset impulse quantity from the beginning of the movement 
 */
void lldEncoderResetDistance (void);

/**
 * @ brief                             Gets wheels current speed [mps]                          
 * @ return  >=0                       Current speed [mps]
 *           -1                        Sensor is not initialized
 */
float lldEncoderGetSpeedMPS (void);


/**
 * @ brief                             Gets wheels current speed [kph]                          
 * @ return  >=0                       Current speed [kph]
 *           -1                        Sensor is not initialized
 */
float lldEncoderGetSpeedKPH (void);

#endif /* INCLUDE_LLD_ENCODER_H_ */
