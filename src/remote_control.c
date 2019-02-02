#include <tests.h>
#include <remote_control.h>

#define icuSteering         PAL_LINE( GPIOE, 5 )
#define icuSpeed            PAL_LINE( GPIOC, 6 )

/***    ICU Steering uses Timer 9 ***/
static ICUDriver *icuSteerDriver      = &ICUD9;

/***    ICU Driving uses Timer 8 ***/
static ICUDriver *icuSpeedDriver      = &ICUD8;


// callback for steering pulses
static void icuWidthcb_steer(ICUDriver *icup)
{
  steer_rc = icuGetWidthX(icup);                // ...X - can work anywhere
                                                // return width in ticks
  chSysLockFromISR();
  chThdResumeI(&trp_rcmode, MSG_OK);            /* Resuming the thread with message.*/
  chSysUnlockFromISR();
}

// callback for speed pulses
static void icuwidthcb_speed(ICUDriver *icup)
{
  speed_rc  = icuGetWidthX(icup);               // ...X - can work anywhere
                                                // return width in ticks
}


/*** Configuration structures for Steering channel ***/

static const ICUConfig icucfg_steer = {
  .mode         = ICU_INPUT_ACTIVE_HIGH,       // Trigger on rising edge
  .frequency    = 1000000,                     // do not depend on PWM freq
  .width_cb     = icuWidthcb_steer,            // callback for Steering
  .period_cb    = NULL,
  .overflow_cb  = NULL,
  .channel      = ICU_CHANNEL_1,               // for Timer
  .dier         = 0
};

/*** Configuration structures for Steering channel ***/

static const ICUConfig icucfg_speed = {
  .mode         = ICU_INPUT_ACTIVE_HIGH,       // Trigger on rising edge
  .frequency    = 1000000,                     // do not depend on PWM freq
  .width_cb     = icuwidthcb_speed,            // callback for Speed
  .period_cb    = NULL,
  .overflow_cb  = NULL,
  .channel      = ICU_CHANNEL_1,               // for Timer
  .dier         = 0
};

static bool         isInitialized       = false;

void remoteControlInit( void )
{
    if ( isInitialized )
            return;

    icuStart( icuSteerDriver, &icucfg_steer );
    palSetLineMode( icuSteering, PAL_MODE_ALTERNATE(3) );
    icuStartCapture( icuSteerDriver );
    icuEnableNotifications( icuSteerDriver );

    icuStart( icuSpeedDriver, &icucfg_speed );
    palSetLineMode( icuSpeed, PAL_MODE_ALTERNATE(3) );
    icuStartCapture( icuSpeedDriver );
    icuEnableNotifications( icuSpeedDriver );

    chThdCreateStatic( waRCModeDetect, sizeof(waRCModeDetect), prio, RCModeDetect, NULL );

    /* Set initialization flag */

    isInitialized = true;
}
