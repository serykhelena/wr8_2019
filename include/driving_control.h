#ifndef INCLUDE_DRIVING_CONTROL_H_
#define INCLUDE_DRIVING_CONTROL_H_

typedef int16_t	   drvctrl_t;


/*
 * @brief   Initialize DriveControl
 */
void DrivingControlInit(void);

/**
 * @brief
 * @note
 */
int16_t DrivingControlGetSpeedRPM (void);

/**
 * @brief
 * @note
 */
float DrivingControlGetSpeedMPS (void);

/**
 * @brief
 * @note
 */
int16_t DrivingControlGetSpeedKPH (void);

/**
 * @ brief                             Gets current distance [metres]
 * @ return  >=0                       Current distance [metres]
 */
int16_t DrivingControlGetDistance (void);

/*
 * @brief   Reset impulse quantity from the beginning of the movement
 */
void DrivingControlResetDistance (void);

#endif /* INCLUDE_DRIVING_CONTROL_H_ */
