#include <tests.h>
#include <rm_control.h>
#include <common.h>

bool         rc_mode  = false;

static thread_reference_t trp_rcmode = NULL;

int32_t     speed_rc = 0;
int32_t     steer_rc = 0;

static void icuwidthcb_speed(ICUDriver *icup)
{
// // palSetLine( LINE_LED2 );

  speed_rc = icuGetWidthX(icup);
}

static void icuwidthcb_steer(ICUDriver *icup)
{
//  palSetLine( LINE_LED2 );

  steer_rc = icuGetWidthX(icup);
  //Entering I-Locked state and resuming the thread, if suspended
  chSysLockFromISR();
  //Resumes a suspended thread
  chThdResumeI(&trp_rcmode, MSG_OK);
  chSysUnlockFromISR();
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
//  palSetLine( LINE_LED2 );

  arg = arg;

  msg_t msg;

  while( true )
  {
//    palSetLine( LINE_LED2 );
    chSysLock();
    msg = chThdSuspendS(&trp_rcmode);
    //msg = chThdSuspendTimeoutS(&trp_rcmode, MS2ST(100));
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


int32_t speedrc = 0;
int32_t steerrc = 0;


void ICUInit(void)
{
//  palSetLine( LINE_LED2 );

  icuStart(&ICUD8, &icucfg_speed);
  palSetPadMode( GPIOC, 6, PAL_MODE_ALTERNATE(3) );
  icuStartCapture(&ICUD8);
  icuEnableNotifications(&ICUD8);

  icuStart(&ICUD9, &icucfg_steer);
  palSetPadMode( GPIOE, 5, PAL_MODE_ALTERNATE(3) );
  icuStartCapture(&ICUD9);
  icuEnableNotifications(&ICUD9);

  chThdCreateStatic( waRCModeDetect, sizeof(waRCModeDetect), NORMALPRIO+1, RCModeDetect, NULL );

//  steer_rc = (steer_rc - 1640)/4.4 ;
//
//  if (speed_rc > 1499)
//    {
////    //  palSetLine( LINE_LED2 );
//    speed_rc = speed_rc - 1500;
//    }
//    else
//    {
////      palSetLine( LINE_LED1 );
//      speed_rc = (speed_rc - 1400)/1.6;
//    }

}

int32_t FetchSpeed (void)
{
//    palSetLine( LINE_LED2 );

  controlValue_t outputPrc = 0;
  if (speed_rc > 1499)
  {
    outputPrc = speed_rc - 1500;
  }
  else
  {
    outputPrc = (speed_rc - 1400)/1.6;
  }
  outputPrc = CLIP_VALUE( outputPrc, -100, 100 );
  return outputPrc;
}

int32_t FetchSteer (void)
{
//    palSetLine( LINE_LED2 );

  controlValue_t outputPrc = (steer_rc - 1640)/4.4 ;
  outputPrc = CLIP_VALUE( outputPrc, -100, 100 );
  return outputPrc;
}
