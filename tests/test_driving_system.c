#include <common.h>
#include <tests.h>
#include <chprintf.h>
#include <lld_control.h>
#include <control_system.h>
#include <driving_control.h>

//#define WORK_NUC
#define WORK_MATLAB

#ifdef WORK_MATLAB
static const SerialConfig sdcfg = {
    .speed  = 115200,
    .cr1 = 0, .cr2 = 0, .cr3 = 0
};
#endif

void testDrivingSystem(void)
{
  controlValue_t spd_ref = 0;
  controlValue_t t_spd_ref = 0;
  controlValue_t delta_speed_ref = 5;
  float t_spd_real = 0;
  controlValue_t t_speed_cs = 0;

  ControlSystemInit(NORMALPRIO);

#ifdef WORK_MATLAB
  int8_t start = 0;
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

#ifdef WORK_MATLAB
    char csd_data = sdGetTimeout(&SD7, TIME_IMMEDIATE); //send to matlab
#else
    char csd_data = sdGetTimeout(&SD3,TIME_IMMEDIATE); //send to nuc
#endif

    switch (csd_data)
    {
    case 'c'://backward
      spd_ref -= delta_speed_ref;
      break;
    case 'd'://forward
      spd_ref += delta_speed_ref;
      break;
    case ' ':
      spd_ref = 0;
      break;
    case 'v':
      spd_ref = -5;//backward
      break;
    case 'f':
      spd_ref = 5;//forward
      break;
//    case 'b':
//      spd_ref = -10;//backward
//      break;
//    case 'g':
//      spd_ref = 10;//forward
//      break;
//    case 'n':
//      spd_ref = -15;//backward
//      break;
//    case 'h':
//      spd_ref = 15;//forward
//      break;
//    case 'm':
//      spd_ref = 20;//forward
//      break;
//    case 'j':
//      spd_ref = -20;//backward
//      break;
#ifdef WORK_MATLAB
    case 'o':
      start = 1;
      break;
#endif
    default: ;
    }

    spd_ref = CLIP_VALUE(spd_ref, -20, 20);
    t_spd_ref = fetchRefSpeed(spd_ref);
    t_spd_real = DrivingControlGetSpeedMPS();
    t_speed_cs = GetSpeedControl();
//    lldControlSetDrivePower(t_speed_cs);

#ifdef WORK_NUC
    dbgprintf("\tRef:(%d)\tReal:(%d)\tSpeed_cs:(%d)\n\r",
              t_spd_ref, (int)(t_spd_real*100), t_speed_cs);
#endif

#ifdef WORK_MATLAB
    if(start == 1)
    {
      sdWrite(&SD7, (uint8_t*) &t_spd_ref, 2);
      int16_t t_spd_rl = (int16_t)(100*t_spd_real);
      sdWrite(&SD7, (uint8_t*) &t_spd_rl, 2);
    }
#endif

    time = chThdSleepUntilWindowed(time, time + MS2ST(25));
  }
}
