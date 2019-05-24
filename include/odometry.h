#ifndef INCLUDE_ODOMETRY_H_
#define INCLUDE_ODOMETRY_H_

typedef int16_t	   odomValue_t;


/*
 * @brief   Initialize Odometry unit
 */
void odometryInit(void);

/**
 * TODO COMMENTS
 */
int16_t odometryGetSpeedRPM (void);

/**
 * TODO COMMENTS
 */
float odometryGetSpeedMPS (void);

/**
 * TODO COMMENTS
 */
float odometryGetLPFSpeedMPS (void);

/**
 * TODO COMMENTS
 */
int16_t odometryGetSpeedKPH (void);

/**
 * @ brief      Gets current distance [m]
 * @ return     Current distance [m]
 */
int16_t odometryGetDistance (void);

/*
 * @brief   Reset impulse quantity from
 *          the beginning of the movement
 */
void odometryResetDistance (void);

#endif /* INCLUDE_ODOMETRY_H_ */
