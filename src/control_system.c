#include <common.h>
#include <tests.h>
#include <control_system.h>
#include <lld_control.h>
#include <lld_steering_control.h>

#define STEER_MAX_RIGHT -29
#define STEER_MAX_LEFT  29

#define STEER_K_LEFT    3.4483
#define STEER_B_LEFT    0

#define STEER_K_RIGHT   3.4483
#define STEER_B_RIGHT   0

systime_t time_cs;

float kp = 0; //proportional

float ki = 0.1; //integral
float sum_i = 0; //sum of errors
//integrator limits
#define MIN_I   -50 //200-300
#define MAX_I    50

float kd = 0; //differential
float old_err = 0; //previous signal value

float angle_ref = 0;
float ang_p = 0;
float ang_i = 0;
float ang_d = 0;
float err = 0;
float angle_real;
int32_t steer_cntrl_prc = 0;



int32_t fetchAngle(controlValue_t ang_dgr)
{
  ang_dgr = CLIP_VALUE(ang_dgr, STEER_MAX_RIGHT, STEER_MAX_LEFT);
  angle_ref = ang_dgr;

  return angle_ref;
}

static THD_WORKING_AREA(waControlSystem, 256);
static THD_FUNCTION(ControlSystem, arg)
{
  arg = arg;
  time_cs = chVTGetSystemTime(); // Current system time.

  while( 1 )
  {

    /*PID REGULATOR FOR STEER*/
    angle_real = lldSteeringControlGetAngle(); //in degrees
    err = angle_ref - angle_real;

    ang_p = kp * err;//proportional

    sum_i = sum_i + err;
    sum_i = CLIP_VALUE(sum_i, MIN_I,MAX_I);
    if (sum_i > MAX_I && sum_i < MIN_I) sum_i = 0;
    ang_i = ki * sum_i;//integral

    ang_d = kd * (err - old_err);//differential
    old_err = err;

    if(angle_ref >= 0)
    steer_cntrl_prc = ang_p + ang_i + ang_d + (angle_ref * STEER_K_LEFT - STEER_B_LEFT);
    else if(angle_ref < 0)
    steer_cntrl_prc = ang_p + ang_i + ang_d + (angle_ref * STEER_K_RIGHT - STEER_B_RIGHT);


//    if (steer_cntrl >= 0)
//      steer_cntrl_prc =  steer_cntrl * STEER_K_LEFT + STEER_B_LEFT;
//    if (steer_cntrl < 0)
//      steer_cntrl_prc =  steer_cntrl * STEER_K_RIGHT + STEER_B_RIGHT;

    steer_cntrl_prc = CLIP_VALUE(steer_cntrl_prc, STEER_MIN, STEER_MAX);
    lldControlSetSteerPower(steer_cntrl_prc);

    /*PID REGULATOR FOR SPEED*/
    time_cs = chThdSleepUntilWindowed(time_cs, time_cs + MS2ST(100));

  }

}

void ControlSystemInit(tprio_t prio)
{
  lldControlInit();
  lldSteeringControlInit();

  chThdCreateStatic( waControlSystem, sizeof(waControlSystem), prio, ControlSystem, NULL );
}

int32_t GetSteerControl (void)
{
  return steer_cntrl_prc;
}
