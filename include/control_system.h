#ifndef INCLUDE_CONTROL_SYSTEM_H_
#define INCLUDE_CONTROL_SYSTEM_H_

typedef int32_t controlValue_t;


int32_t fetchAngle(controlValue_t ang_dgr);

/**
 * @brief
 * @note
 */
void ControlSystemInit(tprio_t prio);

/**
 * @brief
 * @note
 */
int32_t GetSteerControl (void);

#endif /* INCLUDE_CONTROL_SYSTEM_H_ */
