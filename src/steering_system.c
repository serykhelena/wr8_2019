#include <tests.h>
#include <lld_control.h>
#include <lld_steering_control.h>

#define STEER_MAX_RIGHT -29
#define STEER_MAX_LEFT  29

#define STEER_K_LEFT    2.9412
#define STEER_B_LEFT    0

#define STEER_K_RIGHT   3.4483
#define STEER_B_RIGHT   0

systime_t flow_time;

float kp = 1; //proportional

float ki = 1; //integral
float sum_i = 0; //sum of errors
//integrator limits
#define MIN_I   -0.1 //200-300
#define MAX_I    0.1

float kd = 1; //differential
float old_err = 0; //previous signal value

float angle_ref = 0;
float ang_p = 0;
float ang_i = 0;
float ang_d = 0;
float err = 0;
float angle_real;
float steer_cntrl_prc = 0;

void fetchAngle(controlValue_t ang_dgr)
{
  ang_dgr = CLIP_VALUE(ang_dgr, STEER_MAX_RIGHT, STEER_MAX_LEFT);
  angle_ref = ang_dgr;
}

static THD_WORKING_AREA(waCSStModeDetect, 256);
static THD_FUNCTION(CSStModeDetect, arg)
{
  arg = arg;

  msg_t msg;

  while( true )
  {
    flow_time = chVTGetSystemTime(); // Current system time.

    angle_real = lldSteeringControlGetAngle(); //in degrees

    err = angle_ref - angle_real;

    ang_p = kp * err;

    sum_i = sum_i + err;
    sum_i = CLIP_VALUE(sum_i, MIN_I,MAX_I);
    if (sum_i > MAX_I && sum_i < MIN_I) sum_i = 0;
    ang_i = ki * sum_i;

    ang_d = kd * (err - old_err);
    old_err = err;

    steer_cntrl_prc = ang_p + ang_i + ang_d;

//    if (steer_cntrl >= 0)
//      steer_cntrl_prc =  steer_cntrl * STEER_K_LEFT + STEER_B_LEFT;
//    if (steer_cntrl < 0)
//      steer_cntrl_prc =  steer_cntrl * STEER_K_RIGHT + STEER_B_RIGHT;

    steer_cntrl_prc = CLIP_VALUE(steer_cntrl_prc, STEER_MIN, STEER_MAX);
    lldControlSetSteerPower(steer_cntrl_prc);
  }

  flow_time = chThdSleepUntilWindowed(flow_time, flow_time + MS2ST(100));
}

void SteerCSInit(tprio_t prio)
{
  lldControlInit();
  lldSteeringControlInit();

  chThdCreateStatic( waCSStModeDetect, sizeof(waCSStModeDetect), prio, CSStModeDetect, NULL );
}

float GetSteerControl (void)
{
  return steer_cntrl_prc;
}
