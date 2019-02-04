#include <tests.h>
#include <rm_control.h>
#include <common.h>

/*===========================================================================*/
/* Mailbox code.                                                             */
/*===========================================================================*/
#define MAILBOX_SIZE 50

static mailbox_t steer_mb;
//static msg_t b_steer[MAILBOX_SIZE];

static mailbox_t speed_mb;
//static msg_t b_speed[MAILBOX_SIZE];


/*===========================================================================*/
/* ICU driver related.                                                       */
/*===========================================================================*/

static void icuwidthcb_steer(ICUDriver *icup)
{
  icucnt_t last_width = icuGetWidthX(icup);

  chSysLockFromISR();
  chMBPostI(&steer_mb, (msg_t)last_width);
  chSysUnlockFromISR();
}

static void icuwidthcb_speed(ICUDriver *icup)
{
  icucnt_t last_width = icuGetWidthX(icup);

  chSysLockFromISR();
  chMBPostI(&speed_mb, (msg_t)last_width);
  chSysUnlockFromISR();
}

static const ICUConfig icucfg_steer = {
  .mode         = ICU_INPUT_ACTIVE_HIGH,
  .frequency    = 100000,
  .width_cb     = icuwidthcb_steer,
  .period_cb    = NULL,
  .overflow_cb  = NULL,
  .channel      = ICU_CHANNEL_1,
  .dier         = 0
};

static const ICUConfig icucfg_speed = {
  .mode         = ICU_INPUT_ACTIVE_HIGH,
  .frequency    = 100000,
  .width_cb     = icuwidthcb_speed,
  .period_cb    = NULL,
  .overflow_cb  = NULL,
  .channel      = ICU_CHANNEL_1,
  .dier         = 0
};

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
}
