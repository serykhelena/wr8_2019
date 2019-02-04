#include <tests.h>
#include <rm_control.h>
#include <chprintf.h>

/*===========================================================================*/
/* Mailbox code.                                                             */
/*===========================================================================*/
#define MAILBOX_SIZE 50

static mailbox_t steer_mb;
static msg_t b_steer[MAILBOX_SIZE];

static mailbox_t speed_mb;
static msg_t b_speed[MAILBOX_SIZE];

/*===========================================================================*/
/* Serial driver related.                                                    */
/*===========================================================================*/
static const SerialConfig sdcfg = {
  .speed = 115200,
  .cr1 = 0,
  .cr2 = USART_CR2_LINEN,
  .cr3 = 0
};

//void SerialInit( void )
//{
//    sdStart( &SD7, &sdcfg );
//    palSetPadMode( GPIOE, 8, PAL_MODE_ALTERNATE(8) );    // TX
//    palSetPadMode( GPIOE, 7, PAL_MODE_ALTERNATE(8) );    // RX
//
//    chprintf(((BaseSequentialStream *)&SD7), "TEST\r");
//}

void MailboxInit( void )
{
  chMBObjectInit(&steer_mb, b_steer, MAILBOX_SIZE);
  chMBObjectInit(&speed_mb, b_speed, MAILBOX_SIZE);
}

void TestRMControl(void)
{
    ICUInit();
//    SerialInit();
    sdStart( &SD7, &sdcfg );
    palSetPadMode( GPIOE, 8, PAL_MODE_ALTERNATE(8) );    // TX
    palSetPadMode( GPIOE, 7, PAL_MODE_ALTERNATE(8) );    // RX

    chprintf(((BaseSequentialStream *)&SD7), "TEST\r");

    MailboxInit();

    msg_t msg;

    while (true)
    {
      if ( chMBFetch(&steer_mb, &msg, TIME_IMMEDIATE) == MSG_OK )
        chprintf(((BaseSequentialStream *)&SD7), "Steer: %d\n", msg);

      if ( chMBFetch(&speed_mb, &msg, TIME_IMMEDIATE) == MSG_OK )
        chprintf(((BaseSequentialStream *)&SD7), "Speed: %d\n", msg);

      chThdSleepMilliseconds( 1 );
    }
}
