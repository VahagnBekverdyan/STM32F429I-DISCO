/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include <string.h>
#include <iostream>
#include <iomanip>
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_io.h"
#include "stm32f429i_discovery_lcd.h"
#include "timer.h"
#include "systick.h"
#include "button.h"
#include "lcd.h"
#include "uart.h"

// FreeRTOS can be configured via ./Middlewares/Third_Party/FreeRTOS/Source/include/FreeRTOSConfig.h
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

#include <string>
#include <sstream>
using namespace std;

volatile int systick_count = 0;
volatile int stuff;
volatile char simulated_input = 0x30;
#define TASK_PRIORITY (1 | portPRIVILEGE_BIT)
TaskHandle_t LCDTaskHandle;
SemaphoreHandle_t xSemaphore;
QueueHandle_t DisplayQueue;
QueueHandle_t TerminalQueue;
SemaphoreHandle_t xSemaphoreMutex;

static void SystemClock_Config(void);

void display_task(void *);
void uart_send_task( void *);
void uart_receive_task( void *);
void pushbutton_task(void *);

int main(void)
{
	HAL_Init();

	// Configure the system clock
	SystemClock_Config();
	SysTick_init ();
	lcd_init();

	xSemaphore = xSemaphoreCreateBinary();
	xSemaphoreGive( xSemaphore );

	// Nicht Gr��e des Pointers!
	DisplayQueue = xQueueCreate( 15, sizeof( uint8_t ) );
	TerminalQueue = xQueueCreate( 15, sizeof( uint8_t ) );

	xSemaphoreMutex = xSemaphoreCreateMutex();
	//xSemaphoreGive( xSemaphoreMutex );

	xTaskCreate( (pdTASK_CODE)uart_send_task, 	   	"uart",     configMINIMAL_STACK_SIZE, 0, TASK_PRIORITY, NULL);

	xTaskCreate( (pdTASK_CODE)uart_receive_task, 	   	"uart",     configMINIMAL_STACK_SIZE, 0, TASK_PRIORITY, NULL);

	xTaskCreate( (pdTASK_CODE)display_task, 	   "laufschrift",     configMINIMAL_STACK_SIZE, 0, TASK_PRIORITY, NULL);

	xTaskCreate( (pdTASK_CODE)pushbutton_task, "button", configMINIMAL_STACK_SIZE, 0, TASK_PRIORITY, NULL);

	vTaskStartScheduler ();
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