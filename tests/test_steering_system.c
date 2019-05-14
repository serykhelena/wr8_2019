#include <tests.h>
#include <steering_system.h>
#include <lld_control.h>
#include <chprintf.h>
#include <common.h>

void testSteeringSystem(void)
{
  controlValue_t test_steer_cs;
  controlValue_t test_ang_ref = 0;
  controlValue_t delta_steer_ref = 1;
  controlValue_t test_ang_real = 0;

  lldControlInit();
  lldSteeringControlInit();
//  SteerCSInit(1);
  debug_stream_init();

  systime_t time = chVTGetSystemTime(); // Current system time

  dbgprintf("TEST\r\n");

  while(true)
  {

    char rc_data = sdGet (&SD3);
    switch (rc_data)
    {
    case 'a':
      test_ang_ref -= delta_steer_ref;
      break;

    case 's':
      test_ang_ref += delta_steer_ref;

    }

//    test_ang_ref = CLIP_VALUE(test_ang_ref, -29, 29);

    test_ang_real = lldSteeringControlGetAngle();
    test_steer_cs = GetSteerControl();
//    lldControlSetSteerPower(test_steer_cs);

    dbgprintf("\tRef_ang:(%d)\tReal_ang:(%d)\tStr_cntrl:(%d)\n\r",
              test_ang_ref, test_ang_real, test_steer_cs);

    time = chThdSleepUntilWindowed(time, time + MS2ST(500));

  }
}
