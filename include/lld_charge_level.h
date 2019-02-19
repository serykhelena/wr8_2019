#ifndef INCLUDE_LLD_CHARGE_LEVEL_H_
#define INCLUDE_LLD_CHARGE_LEVEL_H_

#include <common.h>
#include <chprintf.h>

void lldChargeLevelInit (void);

int16_t lldChargeLevelGetAdcVal_STMPower (void);

int16_t lldChargeLevelGetCharge_STMPower (void);

int16_t lldChargeLevelGetAdcVal_Battery (void);

int16_t lldChargeLevelGetCharge_Battery (void);

#endif /* INCLUDE_LLD_CHARGE_LEVEL_H_ */
