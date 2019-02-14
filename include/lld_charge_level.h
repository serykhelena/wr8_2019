#ifndef INCLUDE_LLD_CHARGE_LEVEL_H_
#define INCLUDE_LLD_CHARGE_LEVEL_H_

#include <common.h>
#include <chprintf.h>

void lldChargeLevelInit (void);

int16_t lldChargeLevelGetAdcVal (void);

//int16_t lldChargeLevelGetAdcVal_Kalman (void);

int16_t lldChargeLevelGetCharge (void);

int8_t lldChargeLevelDiodsQuantity (void);

void lldChargeLevelDisplay (void);


#endif /* INCLUDE_LLD_CHARGE_LEVEL_H_ */
