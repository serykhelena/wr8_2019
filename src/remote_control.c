#include <tests.h>
#include <remote_control.h>
#include <lld_control.h>

#define icuSteering         PAL_LINE( GPIOE, 5 )
#define icuSpeed            PAL_LINE( GPIOC, 6 )

/***    ICU Steering uses Timer 9 ***/
static ICUDriver *icuSteerDriver      = &ICUD9;

/***    ICU Driving uses Timer 8 ***/
static ICUDriver *icuSpeedDriver      = &ICUD8;

/***    width in ticks from RC for steering wheels  ***/
icuValue_t  steer_rc    =   0;
/***    width in ticks from RC for driving wheels   ***/
icuValue_t  speed_rc    =   0;
/***    flag for RC mode                            ***/
bool        rc_mode     = false;

static thread_reference_t trp_rcmode = NULL;

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
static void icuWidthcb_speed(ICUDriver *icup)
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
  .width_cb     = icuWidthcb_speed,            // callback for Speed
  .period_cb    = NULL,
  .overflow_cb  = NULL,
  .channel      = ICU_CHANNEL_1,               // for Timer
  .dier         = 0
};


static THD_WORKING_AREA(waRCModeDetect, 128); // 128 - stack size
static THD_FUNCTION(RCModeDetect, arg)
{
    arg = arg;

    msg_t msg_mode;     // var for mode detection

    while( true )
    {
      /* Waiting for the IRQ to happen */
      chSysLock();
      msg_mode = chThdSuspendTimeoutS(&trp_rcmode, MS2ST(100)); // 100 ms timeout
      chSysUnlock();

      if(msg_mode == MSG_OK)
      {
        rc_mode = true;
        palSetLine( LINE_LED3 );
      }
      else if(msg_mode == MSG_TIMEOUT)
      {
        rc_mode = false;
        palClearLine( LINE_LED3 );
      }
    }
}

static bool         isInitialized       = false;

/**
 * @brief   Initialize periphery connected to remote control
 * @note    Stable for repeated calls
 * @param   prio defines priority of inside thread
 *          IMPORTANT! NORMALPRIO + prio
 */
void remoteControlInit( int32_t prio )
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

    chThdCreateStatic( waRCModeDetect, sizeof(waRCModeDetect), NORMALPRIO + prio , RCModeDetect, NULL );

    /* Set initialization flag */

    isInitialized = true;
}


/**
 * @brief   Make RC Mode enable
 * @note    IMPORTANT!
 *          Need to be called through constant period of time
 *          strictly!
 */
void remoteModeControl( void )
{
    if( rc_mode )
    {
        lldControlSetDrMotorRawPower( speed_rc );
        lldControlSetSteerMotorPower( steer_rc );
    }
}

/**
 * @brief   Return speed control signal (width) in ticks
 */
icuValue_t rcSpeedControlSignalShow( void )
{
    if( rc_mode ) return speed_rc;
}

/**
 * @brief   Return steering control signal (width) in ticks
 */
icuValue_t rcSteerControlSignalShow( void )
{
    if( rc_mode ) return steer_rc;
}
