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

void testEncoder (void);

/****************************/
/*** Driver Control tests ***/
/****************************/

/*
 * @brief   In test: control and get values of speed and angle of rotation
 * @note    The routine has internal infinite loop
 * @note    SD7 is used for testing (PE7, PE8)
 */

void testDriverControlRoutine (void);


/*************************/
/*** Tests application ***/
/*************************/

/**
 * @brief   Routines of tests
 */
static inline void testsRoutines( void )
{
  #if (MAIN_PROGRAM_ROUTINE == PROGRAM_ROUTINE_TEST_ENCODER)

    testEncoder();

  #elif (MAIN_PROGRAM_ROUTINE == PROGRAM_ROUTINE_TEST_LL_DRIVER)

    testDriverControlRoutine();

  #endif
}

#endif /* INCLUDE_TESTS_H_ */
