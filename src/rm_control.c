#include <tests.h>
#include <rm_control.h>
#include <common.h>

bool         rc_mode  = false;

static thread_reference_t trp_rcmode = NULL;

controlValue_t     speed_rc = 0;
controlValue_t     steer_rc = 0;

static void icuwidthcb_speed(ICUDriver *icup)
{
  controlValue_t temp_speed = 0;
  temp_speed = icuGetWidthX(icup);

  /*** Add limitations for width control  ***/
  if (temp_speed > 1240 && temp_speed < 1600)
      {
      speed_rc = temp_speed;
      }
}

static void icuwidthcb_steer(ICUDriver *icup)
{
  controlValue_t temp_steer = 0;
  temp_steer = icuGetWidthX(icup);
  //Entering I-Locked state and resuming the thread, if suspended
  /*** Add limitations for width control  ***/
  if (temp_steer < 2080 && temp_steer > 1200)
      {
      steer_rc = temp_steer;

      chSysLockFromISR();
       //Resumes a suspended thread
       chThdResumeI(&trp_rcmode, MSG_OK);
       chSysUnlockFromISR();
      }

}

ICUConfig icucfg_speed = {
  .mode         = ICU_INPUT_ACTIVE_HIGH,
  .frequency    = 1000000,
  .width_cb     = icuwidthcb_speed,
  .period_cb    = NULL,
  .overflow_cb  = NULL,
  .channel      = ICU_CHANNEL_1,
  .dier         = 0
};

ICUConfig icucfg_steer = {
  .mode         = ICU_INPUT_ACTIVE_HIGH,
  .frequency    = 1000000,
  .width_cb     = icuwidthcb_steer,
  .period_cb    = NULL,
  .overflow_cb  = NULL,
  .channel      = ICU_CHANNEL_1,
  .dier         = 0
};

static THD_WORKING_AREA(waRCModeDetect, 128);
static THD_FUNCTION(RCModeDetect, arg)
{
  arg = arg;

  msg_t msg;

  while( true )
  {
    chSysLock();
    msg = chThdSuspendTimeoutS(&trp_rcmode, MS2ST(100));
    //Suspends the invoking thread on a reference variable with a 100 ms timeout specification
    chSysUnlock();

    if(msg == MSG_OK)
    {
      rc_mode = true;
      palSetLine( LINE_LED3 );
    }
    else if(msg == MSG_TIMEOUT)
    {
      rc_mode = false;
      palClearLine( LINE_LED3 );
    }
  }
}

//controlValue_t outputPrc_speed = 0;
//controlValue_t outputPrc_steer = 0;

void ICUInit(void)
{
  icuStart(&ICUD8, &icucfg_speed);
  palSetPadMode( GPIOC, 6, PAL_MODE_ALTERNATE(3) );
  icuStartCapture(&ICUD8);
  icuEnableNotifications(&ICUD8);

  icuStart(&ICUD9, &icucfg_steer);
  palSetPadMode( GPIOE, 5, PAL_MODE_ALTERNATE(3) );
  icuStartCapture(&ICUD9);
  icuEnableNotifications(&ICUD9);

  chThdCreateStatic( waRCModeDetect, sizeof(waRCModeDetect), NORMALPRIO+1, RCModeDetect, NULL );
}

controlValueICU_t FetchSpeed (void)
{
  controlValue_t outputPrc = 0;
  if (speed_rc >= 1600 || speed_rc <= 1240)
    {
      speed_rc = 1490;
    }

  if (speed_rc >= 1500 && speed_rc <= 1600)
  {
    outputPrc = speed_rc - 1500;
  }
  else if (speed_rc >= 1240 && speed_rc <= 1400)
  {
    outputPrc = (speed_rc - 1400)/1.6;
  }
  else if (speed_rc > 1400 && speed_rc < 1500)
  {
    outputPrc = 0;
  }

  outputPrc = CLIP_VALUE( outputPrc, -100, 100 );
  return outputPrc;
}

controlValueICU_t FetchSteer (void)
{
  if (steer_rc >= 2080 || steer_rc <= 1200)
        {
        steer_rc = 1630;
        }
  controlValue_t outputPrc = (steer_rc - 1630)/4.4 ;

  outputPrc = CLIP_VALUE( outputPrc, -100, 100 );
  return outputPrc;
}


uint32_t icuRawSteerInput( void )
{
  return steer_rc;

}

uint32_t icuRawSpeedInput( void )
{
  return speed_rc;

}
