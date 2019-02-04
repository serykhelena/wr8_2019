#ifndef INCLUDE_TESTS_H_
#define INCLUDE_TESTS_H_

#include <common.h>

/****************************/
/*** Driver Control tests ***/
/****************************/

/*
 * @brief   In test: get values angle of rotation and position of the front wheels
 * @note    The routine has internal infinite loop
 * @note    SD7 is used for testing (PE7, PE8)
 */

void TestRMControl (void);


static inline void testsRoutines( void )
{
#if (MAIN_PROGRAM_ROUTINE == PROGRAM_ROUTINE_TEST_RM_CONTROL)

  TestRMControl();


#endif
}

#endif /* INCLUDE_TESTS_H_ */
