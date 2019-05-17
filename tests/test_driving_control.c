#include <common.h>
#include <tests.h>
#include <chprintf.h>
#include <driving_control.h>
#include <lld_control.h>
#include <lld_encoder.h>

#define DEBUG

float speedMPS = 0;
encValue_t encSpeedRPM = 0;
int16_t speed = 0;
int8_t delta = 1;


void testDrivingControl()
{
    #ifdef DEBUG
          debug_stream_init();
          dbgprintf("TEST\r\n");
    #endif

	DrivingControlInit();
    lldControlInit( );
    lldEncoderSensInit();

	systime_t time = chVTGetSystemTime(); // Current system time

    while(1)
    {
         #ifdef DEBUG
               char data = sdGetTimeout( &SD3, TIME_IMMEDIATE );;
         #endif

               switch ( data )
               {
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

        encSpeedRPM = lldEncoderGetSpeedMPS();
        speedMPS    = DrivingControlGetSpeedMPS();

        #ifdef DEBUG
        	dbgprintf("Cntrl:(%d)\teRPM(%d)\tMPS(%d)\n\r", (int)speed , (int)encSpeedRPM, (int)(speedMPS * 100));
        #endif
    time = chThdSleepUntilWindowed(time, time + MS2ST(100));
    }
}
