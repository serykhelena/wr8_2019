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

/****************************/
/*** Remote Control tests ***/
/****************************/

/*
 * @brief   In test:
 * @note    The routine has internal infinite loop
 * @note    SD7 is used for testing (PE7, PE8)
 */

void TestRMControl (void);

/*************************/
/*** Tests application ***/
/*************************/

void TestRMControl2 (void);
/**
 * @brief   Routines of tests
 */
static inline void testsRoutines( void )
{
  #if (MAIN_PROGRAM_ROUTINE == PROGRAM_ROUTINE_TEST_ENCODER)
    testEncoder();
  #elif (MAIN_PROGRAM_ROUTINE == PROGRAM_ROUTINE_TEST_LL_DRIVER)
    testDriverControlRoutine();
  #elif (MAIN_PROGRAM_ROUTINE == PROGRAM_ROUTINE_TEST_RM_CONTROL)
    TestRMControl();
  #endif
}

#endif /* INCLUDE_TESTS_H_ */
