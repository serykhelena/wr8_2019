#ifndef INCLUDE_LLD_STEERING_CONTROL_H_
#define INCLUDE_LLD_STEERING_CONTROL_H_

//#include <chprintf.h>
//#include <common.h>

/*
 * @brief   Initialize front wheels control
 */
void lldSteerRotateInit (void);

/*
 * @brief   Get ADC value
 * @return  ADC value from 0 to 4095 equal front wheels position
 */
int16_t GetFrontWheelAdcPos_filt (void);
int16_t GetFrontWheelAdcPos_doublefilt (void);
/*
 * @brief   Get front wheels position  ranging from -100 to 100
 * @return  Value from -100 to 100 equal front wheels position
 */
int16_t GetFrontWheelVal (void);

/*
 * @brief   Get angle of rotate
 * @return  Angle of wheels rotate
 */
int16_t GetFrontWheelAngle (void);

#endif /* INCLUDE_LLD_STEERING_CONTROL_H_ */