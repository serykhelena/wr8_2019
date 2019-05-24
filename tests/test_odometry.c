#include <common.h>
#include <tests.h>
#include <chprintf.h>
#include <odometry.h>
#include <lld_control.h>
#include <lld_encoder.h>

//#define DEBUG
#define MATLAB

#ifdef MATLAB
static const SerialConfig sdcfg = {
  .speed = 115200,
  .cr1 = 0,
  .cr2 = 0,
  .cr3 = 0
};
#endif



/*
 * TODO COMMENTS
 */
void testOdometry()
{
#ifdef DEBUG
          debug_stream_init();

          encValue_t encSpeedRPM      = 0;
#endif

	odometryInit();
    lldControlInit( );
    lldEncoderSensInit();

    float speedMPS              = 0;
    float speedMPS_lpf          = 0;

    int16_t speed               = 0;
    int8_t delta                = 1;

#ifdef MATLAB
      sdStart( &SD7, &sdcfg );
  	  palSetPadMode( GPIOE, 8, PAL_MODE_ALTERNATE(8) );    // TX
  	  palSetPadMode( GPIOE, 7, PAL_MODE_ALTERNATE(8) );    // RX

  	  uint8_t matlab_start_flag = 0;
  	  int16_t matlab_speed      = 0;
  	  int16_t matlab_speed_lpf  = 0;
#endif

	systime_t time = chVTGetSystemTime(); // Current system time

    while(1)
    {
#ifdef DEBUG
         char data = sdGetTimeout( &SD3, TIME_IMMEDIATE );
#endif
#ifdef MATLAB
         char data = sdGetTimeout(&SD7, TIME_IMMEDIATE);
#endif
         switch ( data )
         {
           case 'a':
             matlab_start_flag = 1;
             break;

           case 'w':   // Positive speed
             speed += delta;
             break;

           case 's':   // Negative speed
             speed -= delta;
             break;

           case ' ':
             speed = 0;
             break;

           default: ;
         }

        speed = CLIP_VALUE( speed, -100, 100 );
        lldControlSetDrivePower(speed);


        speedMPS        = odometryGetSpeedMPS();
        speedMPS_lpf    = odometryGetLPFSpeedMPS( );

#ifdef DEBUG
        encSpeedRPM     = lldEncoderGetSpeedMPS();
        dbgprintf("Cntrl:(%d)\teRPM(%d)\tMPS(%d)\n\r",
                  (int)speed , (int)encSpeedRPM,
                  (int)(speedMPS * 100));

        time = chThdSleepUntilWindowed(time, time + MS2ST(100));
#endif

#ifdef MATLAB
        if( matlab_start_flag )
        {
          matlab_speed      = (int)( speedMPS * 100 );
          matlab_speed_lpf  = (int)( speedMPS_lpf * 100 );

          sdWrite( &SD7, (uint8_t*) &matlab_speed, 2);
          sdWrite( &SD7, (uint8_t*) &matlab_speed_lpf, 2);
        }

        time = chThdSleepUntilWindowed(time, time + MS2ST(20));
#endif
    }
}
