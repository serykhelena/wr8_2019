#ifndef INCLUDE_TESTS_H_
#define INCLUDE_TESTS_H_

#include <common.h>

/****************************/
/*** Driver Control tests ***/
/****************************/

/*
 * @brief   Routine of low level driver control testing
 * @note    The routine has internal infinite loop
 * @note    Changing raw values of pwm dutycycle
 */
void testRawDrivingWheelControlRoutine( void );

/*
 * @brief   Routine of low level driver control testing
 * @note    The routine has internal infinite loop
 */
void testWheelsControlRoutines( void );

/*********************/
/*** Encoder tests ***/
/*********************/


/*
 * @brief   Routine of encoder testing
 * @note    The routine has internal infinite loop
 */
void testEncoderRoutine( void );


static inline void testsRoutines( void )
{
#if (MAIN_PROGRAM_ROUTINE == PROGRAM_ROUTINE_TEST_LL_DRIVER)

    testWheelsControlRoutines( );

#elif ( MAIN_PROGRAM_ROUTINE == PROGRAM_ROUTINE_TEST_ENCODER )

    testEncoderRoutine( );

#endif
}

#endif /* INCLUDE_TESTS_H_ */
