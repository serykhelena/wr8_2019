#ifndef INCLUDE_LLD_CHARGE_LEVEL_H_
#define INCLUDE_LLD_CHARGE_LEVEL_H_

#include <common.h>
#include <chprintf.h>

/*
 * @brief   Initialize charge level module
 */
void lldChargeLevelInit (void);

/*
 * @brief   Get ADC ¹ 3 value from A3(AN9)
 * @return  ADC value from 0 to 4095 equal voltage for STM power (2cell-battery)
 */
int16_t lldChargeLevelGetAdcVal_STMPower (void);

/*
 * @brief 	Get current 2cell-battery power
 * @return  2cell-battery percentage from 0 to 100
 */
int16_t lldChargeLevelGetCharge_STMPower (void);

/*
 * @brief   Get ADC ¹ 3 value from A4(AN15)
 * @return  ADC value from 0 to 4095 equal voltage from main battery (4cell-battery)
 */
int16_t lldChargeLevelGetAdcVal_Battery (void);

/*
 * @brief 	Get current 4cell-battery power
 * @return  4cell-battery percentage from 0 to 100
 */
int16_t lldChargeLevelGetCharge_Battery (void);

#endif /* INCLUDE_LLD_CHARGE_LEVEL_H_ */
