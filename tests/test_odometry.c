#include <common.h>
#include <tests.h>
#include <chprintf.h>
#include <odometry.h>
#include <lld_control.h>
#include <lld_encoder.h>

//#define DEBUG
//#define TERMINAL
//#define MATLAB


float speedMPS = 0;
encValue_t encSpeedRPM = 0;
int16_t speed = 0;
int8_t delta = 1;

static const SerialConfig sdcfg = {
  .speed = 115200,
  .cr1 = 0,
  .cr2 = 0,
  .cr3 = 0
};

void serial_init(void)
{
    sdStart( &SD7, &sdcfg );
    palSetPadMode( GPIOE, 8, PAL_MODE_ALTERNATE(8) );    // TX
    palSetPadMode( GPIOE, 7, PAL_MODE_ALTERNATE(8) );    // RX
}

/*
 * TODO COMMETNS
 */
void testOdometry()
{
    #ifdef DEBUG
          debug_stream_init();
          dbgprintf("TEST\r\n");
    #endif

    #ifdef TERMINAL
          serial_init();
          chprintf((BaseSequentialStream *)&SD7, "TEST\r\n");
    #endif

    OdometryInit();
    lldControlInit( );
    lldEncoderSensInit();

    #ifdef TERM
      sdStart( &SD7, &sdcfg );
      palSetPadMode( GPIOE, 8, PAL_MODE_ALTERNATE(8) );    // TX
      palSetPadMode( GPIOE, 7, PAL_MODE_ALTERNATE(8) );    // RX
    #endif

    systime_t time = chVTGetSystemTime(); // Current system time

    while(1)
    {
         #ifdef DEBUG
               char data = sdGetTimeout( &SD3, TIME_IMMEDIATE );
         #else
               char data = sdGetTimeout(&SD7, TIME_IMMEDIATE);
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
        speedMPS    = OdometryGetSpeedMPS();

        #ifdef DEBUG
            dbgprintf("Cntrl:(%d)\teRPM(%d)\tMPS(%d)\n\r", (int)speed , (int)encSpeedRPM, (int)(speedMPS * 100));
        #else
            chprintf( (BaseSequentialStream *)&SD7, "Cntrl:(%d)\teRPM(%d)\tMPS(%d)\n\r", (int)speed , (int)encSpeedRPM, (int)(speedMPS * 100));
        #endif

        #ifdef TERMINAL
            chprintf((BaseSequentialStream *)&SD7, "speed (%d) \teRPM(%d)\tMPS(%d)\n\r", (int)speed, (int)encSpeedRPM, (int)(speedMPS * 100));
        #endif
    time = chThdSleepUntilWindowed(time, time + MS2ST(100));
    }
}

#define MATLAB

/*
 * TODO COMMETNS
 */
void testSpeed_Distance(void)
{
#ifdef TERMINAL
    float DistTest      = 0;
#endif
#ifdef DEBUG
    debug_stream_init();
#endif
    float SpeedTest     = 0;
    float FSpeedTest    = 0;

#ifdef MATLAB
    uint8_t permission  = 0;
	int16_t MLABSpeed   = 0;
	int16_t MLABFSpeed  = 0;
#endif
	int32_t  cntr_speed  = 0;

	serial_init();
	OdometryInit();
    lldControlInit( );

	systime_t time = chVTGetSystemTime(); // Current system time
	while( 1 )
	{
#ifdef DEBUG
	    char rc_data = sdGetTimeout(&SD3, TIME_IMMEDIATE);
#else
        char rc_data = sdGetTimeout(&SD7, TIME_IMMEDIATE);
#endif
        switch( rc_data )
        {
            case 'z':   // Positive speed
              cntr_speed = 10;
              break;

            case 's':
              cntr_speed = -10;
              break;

            case ' ':
              cntr_speed = 0;
              break;

#ifdef MATLAB
            case 'a':
              permission = 1;
              break;

            case 'x':
              permission = 0;
              break;
#endif

            default: ;
        }

        cntr_speed = CLIP_VALUE( cntr_speed, -100, 100 );
        lldControlSetDrivePower(cntr_speed);

#ifdef TERMINAL
		DistTest = OdometryGetDistance();
#endif
		SpeedTest   = OdometryGetSpeedSmPS();
		FSpeedTest  = OdometryGetLPFSpeedSmPS();

#ifdef TERMINAL
		chprintf((BaseSequentialStream *)&SD7, "dist:(%d)\tspeed:(%d)\tfspeed:(%d)\n\r",
				(int)(DistTest), (int)SpeedTest, (int)FSpeedTest);

		time = chThdSleepUntilWindowed(time, time + MS2ST(300));
#endif

#ifdef DEBUG
		dbgprintf("SP:(%d)\tLPF:(%d)\tCNTR:(%d)\tdata:(%c)\n\r",
		          (int)SpeedTest, (int)FSpeedTest, cntr_speed, rc_data);

		time = chThdSleepUntilWindowed(time, time + MS2ST(300));
#endif

#ifdef MATLAB
		if( permission == 1 )
		{

			MLABSpeed = (int)SpeedTest;
			MLABFSpeed = (int)FSpeedTest;

			sdWrite(&SD7, (uint8_t*) &MLABSpeed, 2);
	    	sdWrite(&SD7, (uint8_t*) &MLABFSpeed, 2);
		}
		time = chThdSleepUntilWindowed(time, time + MS2ST(10));
#endif
//		time = chThdSleepUntilWindowed(time, time + MS2ST(10));
	}
}
