#include <ch.h>
#include <tests.h>
#include <lld_encoder.h>

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

void testEncoder (void)
{
	sd_set();
    lldEncoderSensInit();

    float SpeedRPM = 0 ;
    float SpeedMPS = 0 ;
    float SpeedKPH = 0 ;
    float fPartRPM = 0 ;
    float fPartMPS = 0 ;
    float fPartKPH = 0 ;
    uint16_t Rev = 0;
    uint32_t Ticks = 0;

    while (true)
    {
    	SpeedRPM = lldEncoderGetSpeedRPM() ;
    	SpeedMPS = lldEncoderGetSpeedMPS();
    	SpeedKPH = lldEncoderGetSpeedKPH();
    	fPartRPM = (SpeedRPM - (int)SpeedRPM) * 100;
		fPartMPS = (SpeedMPS - (int)SpeedMPS) * 100;
		fPartKPH = (SpeedKPH - (int)SpeedKPH) * 100;
        Rev = lldEncoderGetRevolutions();
        Ticks = lldEncoderGetEncTicks();

    	chprintf((BaseSequentialStream *)&SD7, "RPM: %d.%02d     MPS: %d.%02d      KPH: %d.%02d      Rev: %d      Ticks: %d\n\r", (int)SpeedRPM, (int)fPartRPM, (int)SpeedMPS, (int)fPartMPS, (int)SpeedKPH, (int)fPartKPH, Rev, Ticks);
    	chThdSleepMilliseconds(300);
    }
}