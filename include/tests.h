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

/*
 * @brief   In test: get charge level from 2cell and 4cell batteries
 * @note    The routine has internal infinite loop
 * @note    SD7 is used for testing (PE7, PE8)
 */
void testChargeLevel (void);

/*
 * @brief   In test: get angle of rotation and front wheels position
 * @note    The routine has internal infinite loop
 * @note    SD7 is used for testing (PE7, PE8)
 */
void testSteeringControl (void);

/*
 * TODO COMMENTS
 */
void testSteeringSystem (void);

/*
 * TODO COMMENTS
 */
void testDrivingSystem (void);

/*
 * TODO COMMENTS
 */
void testOdometry (void);

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

  #elif (MAIN_PROGRAM_ROUTINE == PROGRAM_ROUTINE_TEST_CHARGE_LEVEL)

    testChargeLevel();
    
  #elif (MAIN_PROGRAM_ROUTINE == PROGRAM_ROUTINE_TEST_STEERING_CONTROL)

    testSteeringControl();

  #elif (MAIN_PROGRAM_ROUTINE == PROGRAM_ROUTINE_TEST_STEERING_SYSTEM)

    testSteeringSystem();

  #elif (MAIN_PROGRAM_ROUTINE == PROGRAM_ROUTINE_TEST_DRIVING_SYSTEM)

    testDrivingSystem();

  #elif (MAIN_PROGRAM_ROUTINE == PROGRAM_ROUTINE_TEST_ODOMETRY)

    testOdometry();

  #endif
}

#endif /* INCLUDE_TESTS_H_ */
