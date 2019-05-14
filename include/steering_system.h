#ifndef INCLUDE_STEERING_SYSTEM_H_
#define INCLUDE_STEERING_SYSTEM_H_

typedef int32_t controlValue_t;
typedef thread_t * thread_reference_t;

/**
 * @brief
 * @note
 */
void ICUInit2(void);

/**
 * @brief
 * @note
 */
float SteerPIDReg (float ang_dgr);

#endif /* INCLUDE_STEERING_SYSTEM_H_ */
