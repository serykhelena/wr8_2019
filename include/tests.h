#ifndef INCLUDE_TESTS_H_
#define INCLUDE_TESTS_H_

#include <common.h>

/****************************/
/*** Driver Control tests ***/
/****************************/

/*
 * @brief   Routine of low level driver control testing
 * @note    The routine has internal infinite loop
 */
void testDriverControlRoutine( void );


static inline void testsRoutines( void )
{
#if (MAIN_PROGRAM_ROUTINE == PROGRAM_ROUTINE_TEST_LL_DRIVER)

    testDriverControlRoutine();

#endif
}

#endif /* INCLUDE_TESTS_H_ */
