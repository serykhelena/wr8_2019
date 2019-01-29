#include <tests.h>
#include <encoder.h>

#define MAX_TICK_NUM        360

rawEncoderValue_t   enc_trigger_counter = 0;
encoderValue_t      enc_rev_number      = 0;

static void extcb1(EXTDriver *extp, expchannel_t channel)
{
    (void)extp;
    (void)channel;
    palToggleLine( LINE_LED1 );

    palSetLine( LINE_LED2 );

    enc_trigger_counter += 1;

    if( enc_trigger_counter > 720)    enc_trigger_counter = 0;
    if( enc_trigger_counter == 720 )  enc_rev_number += 1;
}

static void extcb2(EXTDriver *extp, expchannel_t channel)
{
   (void)extp;
   (void)channel;
}

static const EXTConfig extcfg =
{
   {
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOD , extcb1}, // PD3
    {EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOD , extcb1}, // PD4
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_FALLING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOC , extcb2}, // C13 - BUT
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL}
  }
};

static bool         isInitialized       = false;

/**
 * @brief   Initialize periphery connected to encoder control
 * @note    Stable for repeated calls
 */
void encoderInit( void )
{
    if ( isInitialized )
            return;

//    EXTConfig extcfg;

    extStart( &EXTD1, &extcfg );
    palSetLine( LINE_LED3 );

    /* Set initialization flag */

    isInitialized = true;
}

/**
 * @brief   Get raw encoder value
 * @return  raw encoder values (ticks)
 */
rawEncoderValue_t getEncoderRawTickNumber( void )
{
    return enc_trigger_counter;
}


/**
 * @brief   Get encoder revs value
 * @return  number of motor revs
 */
encoderValue_t getEncoderRevNumber( void )
{
//    if( enc_trigger_counter == 720 )    enc_rev_number += 1;

    return enc_rev_number;
}


