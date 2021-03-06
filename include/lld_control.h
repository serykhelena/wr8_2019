#ifndef INCLUDE_LLD_CONTROL_H_
#define INCLUDE_LLD_CONTROL_H_

typedef int32_t controlValue_t;
//typedef float ControlValue_t;

/**
 * @brief   Initialize periphery connected to driver control
 * @note    Stable for repeated calls
 */
void lldControlInit ( void );

/**
 * @brief   Set power for driving motor
 * @param [in]  a   Motor power value [-100 100]
 */
void lldControlSetDrivePower(controlValue_t inputPrc);

/*
 * @brief   Set power for steering motor (via ESC)
 * @param   a   Motor power value [-100 100]
 * @note    power (0, 100]  -> clockwise
 * @note    power [-100, 0} -> counterclockwise
 */
void lldControlSetSteerPower(controlValue_t inputPrc);

//void PrintKBsteer(void);
//void PrintKBspeed(void);


#endif /* INCLUDE_LLD_CONTROL_H_ */
