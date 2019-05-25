#ifndef INCLUDE_ODOMETRY_H_
#define INCLUDE_ODOMETRY_H_

typedef int16_t	   drvctrl_t;


/*
 * @brief   Initialize DriveControl
 */
void OdometryInit(void);

/**
 * @brief
 * @note
 */
int16_t OdometryGetSpeedRPM (void);

/**
 * @brief
 * @note
 */
float OdometryGetSpeedMPS (void);

/**
 * @brief
 * @note
 */
float OdometryGetSpeedSmPS (void);

/**
 * @brief
 * @note
 */
float OdometryGetLPFSpeedSmPS (void);



int16_t OdometryGetSpeedKPH (void);


/**
 * @ brief                             Gets current distance [metres]
 * @ return  >=0                       Current distance [metres]
 */
float OdometryGetDistance (void);

/*
 * @brief   Reset impulse quantity from the beginning of the movement
 */
void OdometryResetDistance (void);

#endif /* INCLUDE_ODOMETRY_H_ */
