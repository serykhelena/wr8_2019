
#ifndef INCLUDE_REMOTE_CONTROL_H_
#define INCLUDE_REMOTE_CONTROL_H_

/*** Variables ***/
typedef int32_t icuValue_t;

/**
 * @brief   Initialize periphery connected to remote control
 * @note    Stable for repeated calls
 * @param   prio defines priority of inside thread
 *          IMPORTANT! NORMALPRIO + prio
 */
void remoteControlInit( int32_t prio );

/**
 * @brief   Make RC Mode enable
 * @note    IMPORTANT!
 *          Need to be called through constant period of time
 *          strictly!
 */
void remoteModeControl( void );

/**
 * @brief   Return speed control signal (width) in ticks
 */
icuValue_t rcSpeedControlSignalShow( void );

/**
 * @brief   Return steering control signal (width) in ticks
 */
icuValue_t rcSteerControlSignalShow( void );

#endif /* INCLUDE_REMOTE_CONTROL_H_ */
