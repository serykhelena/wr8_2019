#include <tests.h>
#include <lld_control.h>
#include <lld_steering_control.h>


#define STEER_MAX_RIGHT 29
#define STEER_MAX_LEFT  34

float kp = 1; //proportional

float ki = 1; //integral
float sum_i = 0; //sum of errors
//integrator limits
#define MIN_I   -0.1 //200-300
#define MAX_I    0.1

float kd = 1; //differential
float old_err = 0; //previous signal value

float ang_p, ang_i, ang_d, err, steer_cntrl;

void fetchAngle(controlValue_t ang_dgr)
{
  ang_dgr = CLIP_VALUE(ang_dgr, STEER_MAX_RIGHT,STEER_MAX_LEFT);
  float angle_ref = ang_dgr;
}

static THD_WORKING_AREA(waCSStModeDetect, 256);
static THD_FUNCTION(CSStModeDetect, arg)
{
  arg = arg;

  msg_t msg;

  while( true )
  {
    systime_t flow_time = chVTGetSystemTime(); // Current system time.

    float angle_real = lldSteeringControlGetAngle(); //in degrees

    err = angle_ref - angle_real;

    ang_p = kp * err;

    sum_i = sum_i + err;
    sum_i = CLIP_VALUE(sum_i, MIN_I,MAX_I);
//    if (sum_i > max_i) sum_i = max_i;
//    if (sum_i < min_i) sum_i = min_i;
    ang_i = ki * sum_i;

    ang_d = kd * (err - old_err);
    old_error = err;

    steer_cntrl = ang_p + ang_i + ang_d;

    steer_cntrl_prc = CLIP_VALUE(steer_cntrl_prc, STEER_MIN, STEER_MAX);


    lldControlSetSteerPower(steer_cntrl_prc);
  }

  flow_time = chThdSleepUntilWindowed(flow_time, flow_time + MS2ST(100));
}

void ICUInit2(void)
{
//  icuStart(&ICUD9, &icucfg_steer);
//  palSetPadMode( GPIOE, 5, PAL_MODE_ALTERNATE(3) );
//  icuStartCapture(&ICUD9);
//  icuEnableNotifications(&ICUD9);

  chThdCreateStatic( waCSStModeDetect, sizeof(waCSStModeDetect), NORMALPRIO+2, CSStModeDetect, NULL );
}


