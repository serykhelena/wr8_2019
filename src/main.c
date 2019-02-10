#include <common.h>
#include <tests.h>
#include <chprintf.h>

static const SerialConfig sdcfg = {
  .speed = 115200,
  .cr1 = 0,
  .cr2 = 0,
  .cr3 = 0
};


void sd_set(void)
{
    sdStart( &SD7, &sdcfg );
    palSetPadMode( GPIOE, 8, PAL_MODE_ALTERNATE(8) );    // TX
    palSetPadMode( GPIOE, 7, PAL_MODE_ALTERNATE(8) );    // RX
}

int main(void)
{
    chSysInit();
    halInit();
    sd_set();

    #if (MAIN_PROGRAM_ROUTINE != PROGRAM_ROUTINE_MASTER)

        testsRoutines();

    #endif
}
