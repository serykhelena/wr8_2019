#ifndef INCLUDE_RM_CONTROL_H_
#define INCLUDE_RM_CONTROL_H_

typedef int32_t controlValue_t;
typedef int32_t controlValueICU_t;
typedef thread_t * thread_reference_t;

/**
 * @brief
 * @note
 */
void ICUInit ( void );

//int32_t FetchSteer (void);
//int32_t FetchSpeed (void);


uint32_t icuRawSteerInput( void );

uint32_t icuRawSpeedInput( void );

controlValueICU_t FetchSteer (void);


#endif /* INCLUDE_LLD_CONTROL_H_ */
