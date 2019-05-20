#include <common.h>
#include <tests.h>
#include <chprintf.h>
#include <control_system.h>
#include <lld_control.h>
#include <lld_steering_control.h>

//#define WORK_NUC
#define WORK_MATLAB

#ifdef WORK_MATLAB
static const SerialConfig sdcfg = {
    .speed  = 115200,
    .cr1 = 0,
    .cr2 = 0,
    .cr3 = 0
};
#endif


void testSteeringSystem(void)
{
  controlValue_t ang_ref_dgr = 0;
  controlValue_t t_ang_ref = 0;
  controlValue_t delta_steer_ref = 1;
  controlValue_t t_ang_real = 0;
  int16_t t_steer_cs;

  ControlSystemInit(NORMALPRIO-1);

#ifdef WORK_MATLAB
  int32_t start = 0;
  sdStart( &SD7, &sdcfg );
  palSetPadMode( GPIOE, 8, PAL_MODE_ALTERNATE(8) );    // TX
  palSetPadMode( GPIOE, 7, PAL_MODE_ALTERNATE(8) );    // RX
#endif

#ifdef WORK_NUC
  debug_stream_init();
  dbgprintf("TEST\r\n");
#endif

  systime_t time = chVTGetSystemTime(); // Current system time

  while(true)
  {
    chprintf(((BaseSequentialStream *)&SD7), "TEST\r\n");

#ifdef WORK_MATLAB
    char cs_data = sdGetTimeout(&SD7, TIME_IMMEDIATE); //send to matlab
#else
    char cs_data = sdGet(&SD3); //send to nuc
#endif

    switch (cs_data)
    {
    case 's':
      ang_ref_dgr -= delta_steer_ref;
      break;

    case 'a'://left
      ang_ref_dgr += delta_steer_ref;
      break;

    case ' ':
      ang_ref_dgr = 0;
      break;

    case 'z':
      ang_ref_dgr = 29;//left
      break;

    case 'x':
      ang_ref_dgr = -29;
      break;

#ifdef WORK_MATLAB
    case 'o':
      start = 1;
      break;
#endif

    default:
      ;
    }

    ang_ref_dgr = CLIP_VALUE(ang_ref_dgr, -29, 29);
    t_ang_ref = fetchAngle(ang_ref_dgr);
    t_ang_real = lldSteeringControlGetAngle();
    t_steer_cs = GetSteerControl();
//    lldControlSetSteerPower(test_steer_cs);


#ifdef WORK_NUC
    dbgprintf("\tRef_ang:(%d)\t Real_ang:(%d)\tStr_cntrl:(%d)\n\r",
              t_ang_ref, t_ang_real, t_steer_cs);
#endif

#ifdef WORK_MATLAB
    if(start == 1)
    {
      sdWrite(&SD7, (uint16_t*) &t_ang_ref, 2);
      sdWrite(&SD7, (uint16_t*) &t_ang_real, 2);

    }
#endif

    time = chThdSleepUntilWindowed(time, time + MS2ST(100));

  }
}
