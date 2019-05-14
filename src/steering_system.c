#include <tests.h>
#include <lld_control.h>
#include <lld_steering_control.h>

#define STEER_MAX_RIGHT 29
#define STEER_MAX_LEFT  34

bool    csst_mode = false;

static thread_reference_t trp_csstmode = NULL;

control_Value steer_cs = 0;

static void icuwidthcb_steer_cs(ICUDriver *icup)
{
  controlValue_t temp_steer_cs = 0;
  temp_steer_cs = icuGetWidthX(icup);
  //Entering I-Locked state and resuming the thread, if suspended
  /*** Add limitations for width control  ***/
  if (temp_steer_cs <= STEER_MAX_RIGHT && temp_steer_cs >= STEER_MAX_LEFT)
      {
      steer_cs = temp_steer_cs;

      chSysLockFromISR();
       //Resumes a suspended thread
       chThdResumeI(&trp_csstmode, MSG_OK);
       chSysUnlockFromISR();
      }
}

ICUConfig icucfg_steer_cs = {
  .mode         = ICU_INPUT_ACTIVE_HIGH,
  .frequency    = 100,
  .width_cb     = icuwidthcb_steer_cs,
  .period_cb    = NULL,
  .overflow_cb  = NULL,
  .channel      = ICU_CHANNEL_2,  //which channel??
  .dier         = 0
};

static THD_WORKING_AREA(waCSStModeDetect, 128);
static THD_FUNCTION(CSStModeDetect, arg)
{
  arg = arg;

  msg_t msg;

  while( true )
  {
    chSysLock();
    msg = chThdSuspendTimeoutS(&trp_csstmode, MS2ST(100));
    //Suspends the invoking thread on a reference variable with a 100 ms timeout specification
    chSysUnlock();

    if(msg == MSG_OK)
    {
      csst_mode = true;
      palSetLine( LINE_LED2 );
    }
    else if(msg == MSG_TIMEOUT)
    {
      csst_mode = false;
      palClearLine( LINE_LED3 );
    }
  }
}

void ICUInit2(void)
{
  icuStart(&ICUD9, &icucfg_steer);
  palSetPadMode( GPIOE, 5, PAL_MODE_ALTERNATE(3) );
  icuStartCapture(&ICUD9);
  icuEnableNotifications(&ICUD9);

  chThdCreateStatic( waCSStModeDetect, sizeof(waCSStModeDetect), NORMALPRIO+2, CSStModeDetect, NULL );
}

float kp = 1; //proportional

float ki = 1; //integral
float sum_i = 0; //sum of errors
//integrator limits
#define min_i   -0.1
#define max_i    0.1

float kd = 1; //differential
float old_error = 0; //previous signal value


float SteerPIDReg (float ang_dgr)
{
  float ang_p, ang_i, ang_d, error, position;

  error = ang_dgr; //feedback RotateAngle
  ang_p = kp * error;

  sum_i = sum_i + error;
  if (sum_i > max_i) sum_i = max_i;
  if (sum_i < min_i) sum_i = min_i;
  ang_i = ki * sum_i;

  ang_d = kd * (error - old_error);
  old_error = error;

  position = ang_p + ang_i + ang_d;

  return position;
}

