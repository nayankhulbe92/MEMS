/**
 *	Keil project for LIS302DL or LIS3DSH accelerometer on STM32F4-Discovery board
 *
 *  Designed to work with STM32F4-Discovery board with onboard device
 *
 *	@author		Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 *
 * On init, if you have old LIS302DL on STM32F4-Discovery board,
 * LED_GREEN and LED_RED will be turned on for 2 seconds,
 * if you have new LIS3DSH on STM32F4-Discovery board,
 * LED_BLUE and LED_ORANGE will be turned on for 2 seconds.
 *
 * If all leds are on, there was an error somewhere.
 */
/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_lis302dl_lis3dsh.h"

/* Accelerometer data structure */
TM_LIS302DL_LIS3DSH_t Axes_Data;
int main(void) {	
	/* Initialize system */
	SystemInit();
	
	/* Init delay */
	TM_DELAY_Init();
	
	/* Initialize LEDs */
	TM_DISCO_LedInit();
	
	/* Detect proper device */
	if (TM_LIS302DL_LIS3DSH_Detect() == TM_LIS302DL_LIS3DSH_Device_LIS302DL) {
		/* Turn on GREEN and RED */
		TM_DISCO_LedOn(LED_GREEN | LED_RED);
		/* Initialize LIS302DL */
		TM_LIS302DL_LIS3DSH_Init(TM_LIS302DL_Sensitivity_2_3G, TM_LIS302DL_Filter_2Hz);
	} else if (TM_LIS302DL_LIS3DSH_Detect() == TM_LIS302DL_LIS3DSH_Device_LIS3DSH) {
		/* Turn on BLUE and ORANGE */
		TM_DISCO_LedOn(LED_BLUE | LED_ORANGE);
		/* Initialize LIS3DSH */
		TM_LIS302DL_LIS3DSH_Init(TM_LIS3DSH_Sensitivity_2G, TM_LIS3DSH_Filter_800Hz);
	} else {
		/* Device is not recognized */
		
		/* Turn on ALL leds */
		TM_DISCO_LedOn(LED_GREEN | LED_RED | LED_BLUE | LED_ORANGE);
		
		/* Infinite loop */
		while (1);
	}
	
	/* Delay for 2 seconds */
	Delayms(2000);

	while (1) {
		/* Read axes data from initialized accelerometer */
		TM_LIS302DL_LIS3DSH_ReadAxes(&Axes_Data);
		
		/* Turn LEDS on or off */
		/* Check X axes */
		int c=0;
		if(Axes_Data.X > 50)
		{
			if(Axes_Data.Y > 50)
				c=1;
			else if(Axes_Data.Y < -50)
				c=2;
			else
				c=3;
		}
		if(Axes_Data.X < -50)
		{
			if(Axes_Data.Y > 50)
				c=4;
			else if(Axes_Data.Y < -50)
				c=5;
			else
				c=6;
		}
		if(Axes_Data.Y > 50)
		{
			if(Axes_Data.X > 50)
				c=1;
			else if(Axes_Data.X < -50)
				c=4;
			else
				c=7;
		}
		if(Axes_Data.Y < -50)
		{
			if(Axes_Data.X > 50)
				c=2;
			else if(Axes_Data.X < -50)
				c=5;
			else
				c=8;
		}
		switch(c)
		{
			case 0: TM_DISCO_LedOff(LED_ALL);
							break;
			case 1: TM_DISCO_LedToggle(LED_RED);
							TM_DISCO_LedToggle(LED_ORANGE);
							TM_DISCO_LedOff(LED_BLUE);
							TM_DISCO_LedOff(LED_GREEN);
							Delay(500*(1200-Axes_Data.X));
							break;
			case 2: TM_DISCO_LedToggle(LED_RED);
							TM_DISCO_LedToggle(LED_BLUE);
							TM_DISCO_LedOff(LED_ORANGE);
							TM_DISCO_LedOff(LED_GREEN);
							Delay(500*(1200-Axes_Data.X));
							break;
			case 3: TM_DISCO_LedToggle(LED_RED);
							TM_DISCO_LedOff(LED_ORANGE);
							TM_DISCO_LedOff(LED_GREEN);
							TM_DISCO_LedOff(LED_BLUE);		
							Delay(500*(1200-Axes_Data.X));
							break;
			case 4: TM_DISCO_LedToggle(LED_GREEN);
							TM_DISCO_LedToggle(LED_ORANGE);
							TM_DISCO_LedOff(LED_RED);
							TM_DISCO_LedOff(LED_BLUE);	
							Delay(500*(1200+Axes_Data.X));
							break;
			case 5: TM_DISCO_LedToggle(LED_GREEN);
							TM_DISCO_LedToggle(LED_BLUE);
							TM_DISCO_LedOff(LED_RED);
							TM_DISCO_LedOff(LED_ORANGE);
							Delay(500*(1200+Axes_Data.X));
							break;
			case 6: TM_DISCO_LedToggle(LED_GREEN);
							TM_DISCO_LedOff(LED_RED);
							TM_DISCO_LedOff(LED_ORANGE);
							TM_DISCO_LedOff(LED_BLUE);
							Delay(500*(1200+Axes_Data.X));
							break;
			case 7: TM_DISCO_LedToggle(LED_ORANGE);
							TM_DISCO_LedOff(LED_RED);
							TM_DISCO_LedOff(LED_GREEN);
							TM_DISCO_LedOff(LED_BLUE);
							Delay(500*(1200-Axes_Data.Y));
							break;
			case 8: TM_DISCO_LedToggle(LED_BLUE);
							TM_DISCO_LedOff(LED_RED);
							TM_DISCO_LedOff(LED_GREEN);
							TM_DISCO_LedOff(LED_ORANGE);
							Delay(500*(1200+Axes_Data.Y));
							break;
		}
	}
}
