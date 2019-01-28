#ifndef INCLUDE_LLD_STEERING_CONTROL_H_
#define INCLUDE_LLD_STEERING_CONTROL_H_

typedef int16_t           steer_angle_t;


/*
 * @brief   Initialize front wheels control
 */
void lldSteeringControlInit (void);

/*
 * @brief   Get ADC value
 * @return  ADC value from 0 to 4095 equal front wheels position
 */
int16_t lldSteeringControlGetAdcVal (void);

/*
 * @brief   Get ADC filtered value
 * @return  ADC value from 0 to 4095 equal front wheels position
 */
int16_t lldSteeringControlGetAdcVal_filt (void);

/*
 * @brief   Get ADC double-filtered value
 * @return  ADC value from 0 to 4095 equal front wheels position
 */
int16_t lldSteeringControlGetAdcVal_doublefilt (void);


/*
 * @brief   Get angle of rotate
 * @return  Angle of wheels rotate
 */
int16_t lldSteeringControlGeAngle (void);

#endif /* INCLUDE_LLD_STEERING_CONTROL_H_ */
