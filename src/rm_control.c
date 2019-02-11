#include <tests.h>
#include <rm_control.h>
#include <common.h>

/*=====================================*/
/*           MAILBOX CONFIG            */
/*=====================================*/
//#define MAILBOX_SIZE 50
//
//static mailbox_t steer_mb;
//static msg_t b_steer[MAILBOX_SIZE];
//
//static mailbox_t speed_mb;
//static msg_t b_speed[MAILBOX_SIZE];

//msg_t msg1;
//msg_t msg2;

bool        rc_mode     = false;
icucnt_t     speed_rc = 0;
icucnt_t     steer_rc = 0;

//static thread_reference_t trp_rcmode = NULL;

//void MailboxInit( void )
//{
//  chMBObjectInit(&steer_mb, b_steer, MAILBOX_SIZE);
//  chMBObjectInit(&speed_mb, b_speed, MAILBOX_SIZE);
//}

void icuwidthcb_speed(ICUDriver *icup)
{
  speed_rc = icuGetWidthX(icup);
  // icucnt_t last_width = icuGetWidthX(icup);

  chSysLockFromISR();
  //chMBPostI(&speed_mb, MSG_OK);
  //chMBPostI(&trp_rcmode, MSG_OK);
  chMBPostI(speed_rc, MSG_OK);
  chSysUnlockFromISR();
}

void icuwidthcb_steer(ICUDriver *icup)
{
  steer_rc = icuGetWidthX(icup);
  // icucnt_t last_width = icuGetWidthX(icup);

  chSysLockFromISR();
  chMBPostI(steer_rc, MSG_OK);
  //chMBPostI(&trp_rcmode, MSG_OK);
  //chMBPostI(&steer_mb, MSG_OK);
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
      palSetLine( LINE_LED2 );
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
  icuStart(&ICUD9, &icucfg_steer);
  palSetPadMode( GPIOE, 5, PAL_MODE_ALTERNATE(3) );
  icuStartCapture(&ICUD9);
  icuEnableNotifications(&ICUD9);

  icuStart(&ICUD8, &icucfg_speed);
  palSetPadMode( GPIOC, 6, PAL_MODE_ALTERNATE(3) );
  icuStartCapture(&ICUD8);
  icuEnableNotifications(&ICUD8);

  // MailboxInit();
}

int FetchSteer (void)
{
  chThdCreateStatic( waRCModeDetectSteer, sizeof(waRCModeDetectSteer), NORMALPRIO, RCModeDetectSteer, NULL );
  return steer_rc;
}

int FetchSpeed (void)
{
  chThdCreateStatic( waRCModeDetectSpeed, sizeof(waRCModeDetectSpeed), NORMALPRIO, RCModeDetectSpeed, NULL );
  return speed_rc;
}

//int FetchSteer (void)
//{
//  if ( chMBFetch(&steer_mb, &msg1, TIME_IMMEDIATE) == MSG_OK )
//  steer_rc = msg1;
//
//  return steer_mb;
//}
//
//int FetchSpeed (void)
//{
//  if ( chMBFetch(&speed_mb, &msg2, TIME_IMMEDIATE) == MSG_OK )
//  {
//    speed_rc = msg2;
//    palToggleLine(LINE_LED1);
//  }
//
//  return speed_mb;
//}
