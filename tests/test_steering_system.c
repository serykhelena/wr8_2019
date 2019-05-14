#include <tests.h>
#include <steering_system.h>
#include <chprintf.h>
#include <common.h>

void testSteeringSystem(void)
{
  float test_steer_cs = 0;
  controlValue_t delta_steer_cs = 1;

  ICUInit2();
  lldControlInit();
  debug_stream_init();

  dbgprintf("TEST STEERING SYSTEM\r");

  while(true)
  {
    char rcv_data_cs = sdGet (&SD7);
    switch (rcv_data_cs)
    {
    case 'a':
      test_steer_cs -= delta_steer_cs;
      break;

    case 's':
      test_steer_cs += delta_steer_cs;

    }



    lldSteeringControlGetAngle();

    dbgprintf("\t Steer(%d)\n\r ", steer_cs);
  }
}
