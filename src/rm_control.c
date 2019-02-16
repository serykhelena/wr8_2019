#include <tests.h>
#include <rm_control.h>
#include <common.h>

bool         rc_mode  = false;
icucnt_t     speed_rc = 0;
icucnt_t     steer_rc = 0;

void icuwidthcb_speed(ICUDriver *icup)
{
  palSetLine( LINE_LED2 );

  speed_rc = icuGetWidthX(icup);

  chSysLockFromISR();
  chMBPostI(speed_rc, MSG_OK);
  chSysUnlockFromISR();
}

void icuwidthcb_steer(ICUDriver *icup)
{
  steer_rc = icuGetWidthX(icup);
                 // icucnt_t last_width = icuGetWidthX(icup);
  chSysLockFromISR();
                //chMBPostI(&trp_rcmode, MSG_OK);
                //chMBPostI(&steer_mb, MSG_OK);
  chMBPostI(steer_rc, MSG_OK);
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

static THD_WORKING_AREA(waRCModeDetectSpeed, 128);
static THD_FUNCTION(RCModeDetectSpeed, arg)
{
  arg = arg;

  msg_t msg;

  while( true )
  {
    chSysLock();
    msg = chThdSuspendTimeoutS(speed_rc, MS2ST(100)); // 100 ms timeout
    chSysUnlock();

    if(msg == MSG_OK)
    {
      rc_mode = true;
      palSetLine( LINE_LED1 );
    }
    else if(msg == MSG_TIMEOUT)
    {
      rc_mode = false;
      palClearLine( LINE_LED3 );
    }

  }
}

static THD_WORKING_AREA(waRCModeDetectSteer, 128);
static THD_FUNCTION(RCModeDetectSteer, arg)
{
  arg = arg;

  msg_t msg;

  while( true )
  {
    chSysLock();
    msg = chThdSuspendTimeoutS(steer_rc, MS2ST(100)); // 100 ms timeout
    chSysUnlock();

    if(msg == MSG_OK)
    {
      rc_mode = true;
      palSetLine( LINE_LED2 );
    }
    else if(msg == MSG_TIMEOUT)
    {
      rc_mode = false;
      palClearLine( LINE_LED3 );
    }

  }
}

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

  chThdCreateStatic( waRCModeDetectSpeed, sizeof(waRCModeDetectSpeed), NORMALPRIO, RCModeDetectSpeed, NULL );
  chThdCreateStatic( waRCModeDetectSteer, sizeof(waRCModeDetectSteer), NORMALPRIO, RCModeDetectSteer, NULL );
}

int FetchSpeed (void)
{
  return speed_rc;
}

int FetchSteer (void)
{
  return steer_rc;
}
