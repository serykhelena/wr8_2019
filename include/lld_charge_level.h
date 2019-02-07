#ifndef INCLUDE_LLD_CHARGE_LEVEL_H_
#define INCLUDE_LLD_CHARGE_LEVEL_H_

void lldChargeLevelInit (void);

int16_t lldChargeLevelGetAdcVal (void);

int16_t lldChargeLevelGetCharge (void);

int8_t lldChargeLevelDisplay (void);


#endif /* INCLUDE_LLD_CHARGE_LEVEL_H_ */