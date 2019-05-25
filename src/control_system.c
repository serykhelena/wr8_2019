#include <common.h>
#include <tests.h>
#include <control_system.h>
#include <lld_control.h>
#include <lld_steering_control.h>
#include <odometry.h>

/**************** STEERING PARAMETERS*******************/
#define STEER_MAX_RIGHT -29
#define STEER_MAX_LEFT  29

#define STEER_K_LEFT    3.4483
#define STEER_B_LEFT    0

#define STEER_K_RIGHT   3.4483
#define STEER_B_RIGHT   0

/************** STEERING COEFFICIENTS******************/
float   kp      = 0;        //proportional
float   ki      = 0.1;      //integral
float   kd      = 0;        //differential

float   sum_i   = 0;        //sum of errors
// Saturation for I
#define MIN_I   -50
#define MAX_I    50

float   old_err     = 0;      //previous signal value

float   angle_ref   = 0;
float   ang_p       = 0;
float   ang_i       = 0;
float   ang_d       = 0;
float   err_ang     = 0;
float   angle_real  = 0;
int32_t steer_cntrl_prc = 0;

/**************** SPEEDING PARAMETERS*******************/
#define SPEED_MAX_FRW       20
#define SPEED_MAX_BCW       -20

#define SPEED_K_FRW         0.1
#define SPEED_B_FRW         0

#define SPEED_K_BCW         1.3
#define SPEED_B_BCW         0

#define SPEED_K_BCW_MINI    0.3
#define SPEED_K_FRW_MINI    0.4

float   speed_ref           = 0;
float   spd_p               = 0;
float   spd_i               = 0;
float   spd_d               = 0;
float   err_spd             = 0;
float   speed_real          = 0;
int32_t speed_cntrl_prc     = 0;

/*********** SPEED (0;10) ***************/
float   kp_spdmin_f         = 0.4;      //proportional
float   ki_spdmin_f         = 0.00125;  //integral
float   sum_i_spdmin_f      = 0;        //sum of errors
// Saturation for I
#define MIN_I_SPDMIN_F      -300
#define MAX_I_SPDMIN_F      300

float   kd_spdmin_f         = 0;        //differential
float   old_err_spdmin_f    = 0;        //previous signal value

/*********** SPEED (-10;0) ***************/
float   kp_spdmin_b         = 0.125;    //proportional
float   ki_spdmin_b         = 0.002;    //integral
float   sum_i_spdmin_b      = 0;        //sum of errors
// Saturation for I
#define MIN_I_SPDMIN_B      -200
#define MAX_I_SPDMIN_B      200

float   kd_spdmin_b         = 0;        //differential
float   old_err_spdmin_b    = 0;        //previous signal value

/*********** SPEED (10;20) ***************/
float   kp_spdmax_f         = 2;    //proportional
float   ki_spdmax_f         = 0.1;    //integral
float   sum_i_spdmax_f      = 0;      //sum of errors
// Saturation for I
#define MIN_I_SPDMAX_F      -60
#define MAX_I_SPDMAX_F      60

float   kd_spdmax_f         = 0.1;      //differential
float   old_err_spdmax_f    = 0;        //previous signal value

/*********** SPEED (-20;-10) ***************/
float   kp_spdmax_b         = 2;        //proportional
float   ki_spdmax_b         = 0.1;    //integral
float   kd_spdmax_b         = 0;        //differential
float   sum_i_spdmax_b      = 0;        //sum of errors
// Saturation for I
#define MIN_I_SPDMAX_B      -65
#define MAX_I_SPDMAX_B      65
float old_err_spdmax_b = 0;   //previous signal value

/*******************************************************/

int32_t fetchAngle(controlValue_t ang_dgr)
{
  ang_dgr = CLIP_VALUE(ang_dgr, STEER_MAX_RIGHT, STEER_MAX_LEFT);
  angle_ref = ang_dgr;

  return angle_ref;
}

int32_t csSetRefSpeed(controlValue_t spd_cm_s)
{
  spd_cm_s = CLIP_VALUE(spd_cm_s, SPEED_MAX_BCW, SPEED_MAX_FRW);
  speed_ref = spd_cm_s;

  return speed_ref;
}

static THD_WORKING_AREA(waControlSystem, 256);
static THD_FUNCTION(ControlSystem, arg)
{
  arg = arg;

  systime_t time_cs = chVTGetSystemTime(); // Current system time

  while( 1 )
  {

    /********PID REGULATOR FOR STEER*********/
    angle_real = lldSteeringControlGetAngle(); //in degrees
    err_ang = angle_ref - angle_real;

    ang_p = kp * err_ang;//proportional

    sum_i = sum_i + err_ang;
    sum_i = CLIP_VALUE(sum_i, MIN_I,MAX_I);
    if (sum_i > MAX_I && sum_i < MIN_I) sum_i = 0;
    ang_i = ki * sum_i;//integral

    ang_d = kd * (err_ang - old_err);//differential
    old_err = err_ang;

    if(angle_ref >= 0)
    steer_cntrl_prc = ang_p + ang_i + ang_d + (angle_ref * STEER_K_LEFT - STEER_B_LEFT);
    else if(angle_ref < 0)
    steer_cntrl_prc = ang_p + ang_i + ang_d + (angle_ref * STEER_K_RIGHT - STEER_B_RIGHT);

    steer_cntrl_prc = CLIP_VALUE(steer_cntrl_prc, STEER_MIN, STEER_MAX);
    lldControlSetSteerPower(steer_cntrl_prc);

    /************PID REGULATOR FOR SPEED***********/
    speed_real = OdometryGetLPFSpeedSmPS();

    if (speed_ref > 0 && speed_ref < 10)
    {
      err_spd = speed_ref - (speed_real);

      spd_p = kp_spdmin_f * err_spd;//proportional

      sum_i_spdmin_f += err_spd;
      sum_i_spdmin_f = CLIP_VALUE(sum_i_spdmin_f, MIN_I_SPDMIN_F,MAX_I_SPDMIN_F);
      if (sum_i_spdmin_f > MAX_I_SPDMIN_F && sum_i_spdmin_f < MIN_I_SPDMIN_F) sum_i_spdmin_f = 0;
      spd_i = ki_spdmin_f * sum_i_spdmin_f;//integral

      spd_d = kd_spdmin_f * (err_spd - old_err_spdmin_f);//differential
      old_err_spdmin_f = err_spd;

      speed_cntrl_prc = spd_p + spd_i + spd_d + (speed_ref * SPEED_K_FRW_MINI - SPEED_B_FRW);
    }
    else if (speed_ref > -10 && speed_ref < 0)
    {
      err_spd = speed_ref - (speed_real);

      spd_p = kp_spdmin_b * err_spd;//proportional

      sum_i_spdmin_b += err_spd;
      sum_i_spdmin_b = CLIP_VALUE(sum_i_spdmin_b, MIN_I_SPDMIN_B,MAX_I_SPDMIN_B);
      if (sum_i_spdmin_b > MAX_I_SPDMIN_B && sum_i_spdmin_b < MIN_I_SPDMIN_B) sum_i_spdmin_b = 0;
      spd_i = ki_spdmin_b * sum_i_spdmin_b;//integral

      spd_d = kd_spdmin_b * (err_spd - old_err_spdmin_b);//differential
      old_err_spdmin_b = err_spd;

      speed_cntrl_prc = spd_p + spd_i + spd_d + (speed_ref * SPEED_K_BCW_MINI - SPEED_B_BCW);
    }
    else if (speed_ref <= -10)
    {
      err_spd = speed_ref - (speed_real);

      spd_p = kp_spdmax_b * err_spd;//proportional

      sum_i_spdmax_b += err_spd;
      sum_i_spdmax_b = CLIP_VALUE(sum_i_spdmax_b, MIN_I_SPDMAX_B,MAX_I_SPDMAX_B);
      if (sum_i_spdmax_b > MAX_I_SPDMAX_B && sum_i_spdmax_b < MIN_I_SPDMAX_B) sum_i_spdmax_b = 0;
      spd_i = ki_spdmax_b * sum_i_spdmax_b;//integral
      sum_i_spdmax_f = 0;

      spd_d = kd_spdmax_b * (err_spd - old_err_spdmax_b);//differential
      old_err_spdmax_b = err_spd;

//      if (err_spd >= -1 && err_spd <= 1)
//      {
//        sum_i_spdmax_b = 0;
//      }

      speed_cntrl_prc = spd_p + spd_i + spd_d;// + (speed_ref * SPEED_K_FRW - SPEED_B_FRW);
    }
    else if( speed_ref >= 10 )
    {
      err_spd = speed_ref - (speed_real);

      spd_p = kp_spdmax_f * err_spd;//proportional

      sum_i_spdmax_f = sum_i_spdmax_f + err_spd;
      sum_i_spdmax_f = CLIP_VALUE(sum_i_spdmax_f, MIN_I_SPDMAX_F,MAX_I_SPDMAX_F);
      if (sum_i_spdmax_f > MAX_I_SPDMAX_F && sum_i_spdmax_f < MIN_I_SPDMAX_F) sum_i_spdmax_f = 0;
      spd_i = ki_spdmax_f * sum_i_spdmax_f;//integral

      spd_d = kd_spdmax_f * (err_spd - old_err_spdmax_f);//differential
      old_err_spdmax_f = err_spd;
      sum_i_spdmax_b = 0;

      speed_cntrl_prc = spd_p + spd_i + spd_d;// + (speed_ref * SPEED_K_FRW - SPEED_B_FRW);
    }
    else if (speed_ref == 0)
    {
      speed_cntrl_prc = 0;
    }

    speed_cntrl_prc = CLIP_VALUE(speed_cntrl_prc, SPEED_MIN, SPEED_MAX);
    lldControlSetDrivePower(speed_cntrl_prc);

    time_cs = chThdSleepUntilWindowed(time_cs, time_cs + MS2ST(20));
  }
}

/*
 * TODO COMMENTS
 */
void ControlSystemInit(tprio_t prio)
{
  lldControlInit();
  lldSteeringControlInit();
  OdometryInit();

  chThdCreateStatic( waControlSystem, sizeof(waControlSystem), prio, ControlSystem, NULL );
}

/*
 * TODO COMMENTS
 */
int32_t GetSteerControl (void)
{
  return steer_cntrl_prc;
}

/*
 * TODO COMMENTS
 */
int32_t GetSpeedControl (void)
{
  return speed_cntrl_prc;
}
