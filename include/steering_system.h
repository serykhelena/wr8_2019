#ifndef INCLUDE_STEERING_SYSTEM_H_
#define INCLUDE_STEERING_SYSTEM_H_

typedef int32_t controlValue_t;


void fetchAngle(controlValue_t ang_dgr);

/**
 * @brief
 * @note
 */
void SteerCSInit(tprio_t prio);

/**
 * @brief
 * @note
 */
float GetSteerControl (void);

#endif /* INCLUDE_STEERING_SYSTEM_H_ */
