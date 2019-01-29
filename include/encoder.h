#ifndef INCLUDE_ENCODER_H_
#define INCLUDE_ENCODER_H_

/*** Variables ***/
typedef uint32_t rawEncoderValue_t;
typedef uint32_t encoderValue_t;

/**
 * @brief   Initialize periphery connected to encoder control
 * @note    Stable for repeated calls
 */
void encoderInit( void );

/**
 * @brief   Get raw encoder value
 * @return  raw encoder values (ticks)
 */
rawEncoderValue_t getEncoderRawTickNumber( void );

/**
 * @brief   Get encoder revs value
 * @return  number of motor revs
 */
encoderValue_t getEncoderRevNumber( void );



#endif /* INCLUDE_ENCODER_H_ */
