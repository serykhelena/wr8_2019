#include <common.h>
#include <tests.h>
#include <control_system.h>
#include <lld_control.h>
#include <lld_steering_control.h>
#include <driving_control.h>

systime_t time_cs;

///**************** STEERING PARAMETERS*******************/
//#define STEER_MAX_RIGHT -29
//#define STEER_MAX_LEFT  29
//
//#define STEER_K_LEFT    3.4483
//#define STEER_B_LEFT    0
//
//#define STEER_K_RIGHT   3.4483
//#define STEER_B_RIGHT   0
//
//float kp = 0;           //proportional
//
//float ki = 0.1;         //integral
//float sum_i = 0;        //sum of errors
////integrator limits
//#define MIN_I   -50     //200-300
//#define MAX_I    50
//
//float kd = 0;           //differential
//float old_err = 0;      //previous signal value
//
//float angle_ref = 0;
//float ang_p = 0;
//float ang_i = 0;
//float ang_d = 0;
//float err_ang = 0;
//float angle_real;
//int32_t steer_cntrl_prc = 0;

/**************** SPEEDING PARAMETERS*******************/
#define SPEED_MAX_FRW  20
#define SPEED_MAX_BCW  -20

#define SPEED_K_FRW    4
#define SPEED_B_FRW    0

#define SPEED_K_BCW    4
#define SPEED_B_BCW    0

float kp_spd = 0;       //proportional

float ki_spd = 0.1;     //integral
float sum_i_spd = 0;    //sum of errors
//integrator limits
#define MIN_I_SPD   -50 //200-300
#define MAX_I_SPD    50

float kd_spd = 0;       //differential
float old_err_spd = 0;  //previous signal value

float speed_ref = 0;
float spd_p = 0;
float spd_i = 0;
float spd_d = 0;
float err_spd = 0;
float speed_real;
int32_t speed_cntrl_prc = 0;
/*******************************************************/

//int32_t fetchAngle(controlValue_t ang_dgr)
//{
//  ang_dgr = CLIP_VALUE(ang_dgr, STEER_MAX_RIGHT, STEER_MAX_LEFT);
//  angle_ref = ang_dgr;
//
//  return angle_ref;
//}

int32_t fetchRefSpeed(controlValue_t speed_ref)
{
  speed_ref = CLIP_VALUE(speed_ref, SPEED_MAX_BCW, SPEED_MAX_FRW);

  return speed_ref;
}

static THD_WORKING_AREA(waControlSystem, 256);
static THD_FUNCTION(ControlSystem, arg)
{
  arg = arg;
  time_cs = chVTGetSystemTime(); // Current system time

  while( 1 )
  {

//    /********PID REGULATOR FOR STEER*********/
//    angle_real = lldSteeringControlGetAngle(); //in degrees
//    err_ang = angle_ref - angle_real;
//
//    ang_p = kp * err_ang;//proportional
//
//    sum_i = sum_i + err_ang;
//    sum_i = CLIP_VALUE(sum_i, MIN_I,MAX_I);
//    if (sum_i > MAX_I && sum_i < MIN_I) sum_i = 0;
//    ang_i = ki * sum_i;//integral
//
//    ang_d = kd * (err_ang - old_err);//differential
//    old_err = err_ang;
//
//    if(angle_ref >= 0)
//    steer_cntrl_prc = ang_p + ang_i + ang_d + (angle_ref * STEER_K_LEFT - STEER_B_LEFT);
//    else if(angle_ref < 0)
//    steer_cntrl_prc = ang_p + ang_i + ang_d + (angle_ref * STEER_K_RIGHT - STEER_B_RIGHT);
//
//    steer_cntrl_prc = CLIP_VALUE(steer_cntrl_prc, STEER_MIN, STEER_MAX);
//    lldControlSetSteerPower(steer_cntrl_prc);

    /************PID REGULATOR FOR SPEED***********/
    speed_real = DrivingControlGetSpeedMPS(); //in m/s from Oleg
    speed_real = speed_real*100;           //in cm/s
    err_spd = speed_ref - speed_real;

    spd_p = kp_spd * err_spd;//proportional

    sum_i_spd = sum_i_spd + err_spd;
    sum_i_spd = CLIP_VALUE(sum_i_spd, MIN_I_SPD,MAX_I_SPD);
    if (sum_i_spd > MAX_I_SPD && sum_i_spd < MIN_I_SPD) sum_i_spd = 0;
    spd_i = ki_spd * sum_i_spd;//integral

    spd_d = kd_spd * (err_spd - old_err_spd);//differential
    old_err_spd = err_spd;

    if(speed_ref >= 0)
    speed_cntrl_prc = spd_p + spd_i + spd_d + (speed_ref * SPEED_K_FRW - SPEED_B_FRW);
    else if(speed_ref < 0)
    speed_cntrl_prc = spd_p + spd_i + spd_d + (speed_ref * SPEED_K_BCW - SPEED_B_BCW);

    speed_cntrl_prc = CLIP_VALUE(speed_cntrl_prc, SPEED_MIN, SPEED_MAX);
    lldControlSetDrivePower(speed_cntrl_prc);


    time_cs = chThdSleepUntilWindowed(time_cs, time_cs + MS2ST(100));

  }

}

void ControlSystemInit(tprio_t prio)
{
  lldControlInit();
  lldSteeringControlInit();

  chThdCreateStatic( waControlSystem, sizeof(waControlSystem), prio, ControlSystem, NULL );
}

//int32_t GetSteerControl (void)
//{
//  return steer_cntrl_prc;
//}

int32_t GetSpeedControl (void)
{
  return speed_cntrl_prc;
}

