#ifndef INCLUDE_LLD_ENCODER_H_
#define INCLUDE_LLD_ENCODER_H_

#include <chprintf.h>

typedef float encValue_t;

/*
 * @brief   Initialize EXT, gpt  
 */
void lldEncoderSensInit (void);


/**
 * @ brief                             Gets current quantity of revolutions
 * @ return  >=0                       Current quantity of revolutions (in 1 revolution - 100 encoder ticks)
 *           -1                        Sensor is not initialized
 */
float lldEncoderGetRevolutions(void);

/**
 * @ brief                             Gets current quantity of absolute revolutions
 * @ return  >=0                       Current quantity of revolutions (in 1 revolution - 1 tick from I channel)
 *           -1                        Sensor is not initialized
 */
uint16_t lldEncoderGetAbsRevolutions(void);


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
encValue_t lldEncoderGetSpeedRPM (void);




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
encValue_t lldEncoderGetSpeedKPH (void);



void lldEncoderResetDistance (void);


#endif /* INCLUDE_LLD_ENCODER_H_ */
