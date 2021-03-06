/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_gyroscope.h"

#include <string>
#include <sstream>
#include <iomanip>
#include <math.h>
#include <stdlib.h>

#define GYRO_FS_DPS 250
#define GYRO_ABS_SAMPLE_MAX 0x7fff
			
static void SystemClock_Config(void);

void printaccel(int line, float value)
{
	std::stringstream output;
	std::string outputstring;
	const char * chararray;
	output.str(std::string());
	output << std::fixed /*<< std::setprecision(4)*/ << std::right << std::setw(10) << value;
	outputstring = "";
	outputstring = output.str();
	chararray = "";
	chararray = outputstring.c_str();
	BSP_LCD_DisplayStringAtLine(line,(uint8_t *) chararray);
}

void GYRO_GetXYZ_dps(float * accel)
{
	BSP_GYRO_GetXYZ(accel);
	accel[0] = accel[0]*250/0x7fff*0.00875;
	accel[1] = accel[1]*250/0x7fff*0.00875;
	accel[2] = accel[2]*250/0x7fff*0.00875;
	// Nullratendrift kompensieren
	// Eigentlich br�uchte man einen Kalman-Filter o.�.
	if(abs(accel[0])<0.03 && abs(accel[1])<0.03 && abs(accel[2])<0.03)
	{
		accel[0] = 0;
		accel[1] = 0;
		accel[2] = 0;
	}
}
int main(void)
{
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	BSP_LCD_Init();
	BSP_LCD_LayerDefaultInit(1, (uint32_t) LCD_FRAME_BUFFER);
	BSP_LCD_SetLayerVisible(1, ENABLE);

	BSP_LCD_SelectLayer(1);
	BSP_LCD_Clear(LCD_COLOR_WHITE);
	BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DisplayOn();

	float accel[3];

	// Gyro returns hundredths of degrees/sec
	// Sensitivity: see page 9 of L3GD20 data sheet
	BSP_GYRO_Init();

	int i = 0;
	float x_offset = 0;
	float y_offset = 0;
	float z_offset = 0;
	int sampleNum = 10000;
	for(int i = 0;i<sampleNum;i++)
	{
		GYRO_GetXYZ_dps(accel);
		x_offset += accel[0];
		y_offset += accel[1];
		z_offset += accel[2];
	}
	x_offset = x_offset/sampleNum;
	y_offset = y_offset/sampleNum;
	z_offset = z_offset/sampleNum;

	uint32_t drifttimestart;
	uint32_t drifttimeend;
	uint32_t drifttimedelta;
	float x_drift = 0;
	float y_drift = 0;
	float z_drift = 0;
	for(int i = 0;i<sampleNum;i++)
	{
		if(i==0)
			drifttimestart = HAL_GetTick();
		else if(i==sampleNum-1)
			drifttimeend = HAL_GetTick();
		GYRO_GetXYZ_dps(accel);
		x_drift += accel[0]-x_offset;
		y_drift += accel[1]-y_offset;
		z_drift += accel[2]-z_offset;
	}
	drifttimedelta = drifttimeend - drifttimestart;
	x_drift = x_drift/drifttimedelta;
	y_drift = y_drift/drifttimedelta;
	z_drift = z_drift/drifttimedelta;

	uint32_t currenttime = 0;
	uint32_t lasttime = 0;
	float timediff = 0;
	float xabs = 0;
	float yabs = 0;
	float zabs = 0;
	while(true)
	{
		currenttime = HAL_GetTick();
		timediff = currenttime - lasttime;
		lasttime = currenttime;
		GYRO_GetXYZ_dps(accel);
		printaccel(1,(accel[0]-x_offset));
		printaccel(2,(accel[1]-y_offset));
		printaccel(3,(accel[2]-z_offset));
		// 5.8 experimentell bestimmt
		xabs += (accel[0]-(x_drift*timediff))*timediff/1000/5.8*90;
		yabs += (accel[1]-(y_drift*timediff))*timediff/1000/5.8*90;
		zabs += (accel[2]-(z_drift*timediff))*timediff/1000/5.8*90;
		printaccel(4,xabs);
		printaccel(5,yabs);
		printaccel(6,zabs);
		i++;
		if(i==100)
		{
			BSP_LCD_ClearStringLine(1);
			BSP_LCD_ClearStringLine(2);
			BSP_LCD_ClearStringLine(3);
			BSP_LCD_ClearStringLine(4);
			BSP_LCD_ClearStringLine(5);
			BSP_LCD_ClearStringLine(6);
		}
		i=i%100;
	}
}

/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow :
 *            System Clock source            = PLL (HSE)
 *            SYSCLK(Hz)                     = 180000000
 *            HCLK(Hz)                       = 180000000
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 4
 *            APB2 Prescaler                 = 2
 *            HSE Frequency(Hz)              = 8000000
 *            PLL_M                          = 8
 *            PLL_N                          = 360
 *            PLL_P                          = 2
 *            PLL_Q                          = 7
 *            VDD(V)                         = 3.3
 *            Main regulator output voltage  = Scale1 mode
 *            Flash Latency(WS)              = 5
 *         The LTDC Clock is configured as follow :
 *            PLLSAIN                        = 192
 *            PLLSAIR                        = 4
 *            PLLSAIDivR                     = 8
 * @param  None
 * @retval None
 *
 * COPYRIGHT(c) 2014 STMicroelectronics
 */
static void SystemClock_Config(void) {
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

	/* Enable Power Control clock */
	__PWR_CLK_ENABLE();

	/* The voltage scaling allows optimizing the power consumption when the device is
	 clocked below the maximum system frequency, to update the voltage scaling value
	 regarding system frequency refer to product datasheet.  */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/*##-1- System Clock Configuration #########################################*/
	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 360;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	HAL_PWREx_ActivateOverDrive();

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
	 clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

	/*##-2- LTDC Clock Configuration ###########################################*/
	/* LCD clock configuration */
	/* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
	/* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN = 192 Mhz */
	/* PLLLCDCLK = PLLSAI_VCO Output/PLLSAIR = 192/4 = 48 Mhz */
	/* LTDC clock frequency = PLLLCDCLK / RCC_PLLSAIDIVR_8 = 48/8 = 6 Mhz */
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
	PeriphClkInitStruct.PLLSAI.PLLSAIN = 192;
	PeriphClkInitStruct.PLLSAI.PLLSAIR = 4;
	PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_8;
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
}

