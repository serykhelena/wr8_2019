#ifndef INCLUDE_LLD_CONTROL_H_
#define INCLUDE_LLD_CONTROL_H_

/*** Variables ***/
typedef int32_t controlValue_t;
typedef int32_t rawPwmValue_t;


/**
 * @brief   Initialize periphery connected to driver control
 * @note    Stable for repeated calls
 */
void lldControlInit ( void );

/**
 * @brief   Set power for driving motor
 * @param   inputPrc   Motor power value [-100 100]
 */
void lldControlSetDrMotorPower( controlValue_t inputPrc );

/**
 * @brief   Set power (in ticks) for driving motor
 * @param   deltaDuty   incrementing / decrementing value to center-pos dutycycle
 */
rawPwmValue_t lldControlSetDrMotorRawPower( rawPwmValue_t deltaDuty );

/**
 * @brief   Set power for steering motor
 * @param   inputPrc   Motor power value [-100 100]
 *                     central position = 0
 */
void lldControlSetSteerMotorPower( controlValue_t inputPrc );




#endif /* INCLUDE_LLD_CONTROL_H_ */
