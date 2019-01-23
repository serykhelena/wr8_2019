#ifndef INCLUDE_TEST_WHEEL_ROTATE_H_
#define INCLUDE_TEST_WHEEL_ROTATE_H_

#include <ext_adc_steering.h>

/*
 * @brief   In test: get values angle of rotation and position of the front wheels
 * @note    The routine has internal infinite loop
 * @note    SD7 is used for testing (PE7, PE8)
 */
void testFrontWheels (void);

#endif /* INCLUDE_TEST_WHEEL_ROTATE_H_ */