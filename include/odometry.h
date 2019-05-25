#ifndef INCLUDE_ODOMETRY_H_
#define INCLUDE_ODOMETRY_H_

typedef int16_t	   drvctrl_t;


/*
 * @brief   Initialize DriveControl
 */
void OdometryInit(void);

/**
 * TODO COMMENTS
 */
int16_t OdometryGetSpeedRPM (void);

/**
 * TODO COMMENTS
 */
float OdometryGetSpeedMPS (void);

/**
 * TODO COMMENTS
 */
float OdometryGetSpeedSmPS (void);

/**
 * TODO COMMENTS
 */
float OdometryGetLPFSpeedSmPS (void);

/**
 * TODO COMMENTS
 */
int16_t OdometryGetSpeedKPH (void);

/**
 * TODO COMMENTS
 */
float OdometryGetDistance (void);

/*
 * @brief   Reset impulse quantity from the beginning of the movement
 */
void OdometryResetDistance (void);

#endif /* INCLUDE_ODOMETRY_H_ */
