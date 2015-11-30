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
QueueHandle_t xQueue;
// SemaphoreHandle_t xSemaphoreMutex;

static void SystemClock_Config(void);
void bargraph_task(void *);
void laufschrift_task(void *);
void uart_task( void *);
void test_task (void *);
void pushbutton_task(void *);
// void lcd_balkenanzeige (void *);
// void lcd_laufschrift (void *);
// void lcd_zeit (void *);

void test_task (void *)
{
  BSP_LED_Init (LED3);
  while (true)
    {
      BSP_LED_Toggle(LED3);
      vTaskDelay (configTICK_RATE_HZ/10);
      BSP_LED_Toggle(LED3);
      vTaskDelay (9*configTICK_RATE_HZ/10);
    }
}

/*void lcd_balkenanzeige (void *)
{
	static std::stringstream output;
	static std::string outputstring;
	static const char * chararray;
	static int i = 0;
	static int direction = 1;
	static char test[] = "123456789ABCDE";
	char first;
	while(true)
	{
		for(int j = 0;j<(int) strlen(test);j++)
		{
			if (j == 0)
				first = test[0];
			if(j < (int)strlen(test)-1)
				test[j] = test[j+1];
			if (j == (int)strlen(test)-1)
				test[j]=first;

		}
		BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
		BSP_LCD_FillRect(0, 0, 240, 24);
		BSP_LCD_SetTextColor(LCD_COLOR_RED);
		if(direction==1)
			BSP_LCD_FillRect(i, 0, 100, 24);
					BSP_LCD_FillRect(i, 0, 100, 24);
		for( volatile long int counter = 0;counter<1000000;counter++)
		  		;
		if(direction == 1)
			i++;
		else
			i--;
		if(i == 140 || i == 0)
		{
			direction++;
			direction = direction %2;
		}
	}
}

void lcd_laufschrift (void *)
{

	static std::stringstream output;
	static std::string outputstring;
	static const char * chararray;
	static char test[] = "123456789ABCDE";
	char first;
	while(true)
	{
		for(int j = 0;j<(int) strlen(test);j++)
		{
			if (j == 0)
				first = test[0];
			if(j < (int)strlen(test)-1)
				test[j] = test[j+1];
			if (j == (int)strlen(test)-1)
				test[j]=first;

		}
		BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
		BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
		BSP_LCD_DisplayStringAtLine (1, (uint8_t *) &test);
		for( volatile long int counter = 0;counter<1000000;counter++)
		  		;
	}
}

void lcd_zeit (void *)
{

	static std::stringstream output;
	static std::string outputstring;
	static const char * chararray;
	static int i = 0;
	static Timer mytimerobject(12,34,56);

	while(1)
	{
		output.str(std::string());
		mytimerobject.setMin(systick_count/1000/60);
		mytimerobject.setSec(systick_count/1000);
		mytimerobject.setHun(systick_count/10);
		output << "Time " << mytimerobject.printtime();
		outputstring = "";
		outputstring = output.str();
		chararray = "";
		chararray = outputstring.c_str();
		BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_DisplayStringAtLine (2, (uint8_t *) chararray);
		i++;
	}

}*/

int main(void)
{
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();
	SysTick_init ();

	xSemaphore = xSemaphoreCreateBinary();

	// Nicht Gr��e des Pointers!
	xQueue = xQueueCreate( 15, sizeof( uint8_t ) );

	// xTaskCreate( (pdTASK_CODE)uart_task, 	   "uart",     configMINIMAL_STACK_SIZE, 0, TASK_PRIORITY, NULL);

	// xTaskCreate( (pdTASK_CODE)test_task, 	   "test",     configMINIMAL_STACK_SIZE, 0, TASK_PRIORITY, NULL);

	xTaskCreate( (pdTASK_CODE)bargraph_task, "bargraph", configMINIMAL_STACK_SIZE, 0, TASK_PRIORITY, &LCDTaskHandle);

	xTaskCreate( (pdTASK_CODE)pushbutton_task, "button", configMINIMAL_STACK_SIZE, 0, TASK_PRIORITY, NULL);

	xTaskCreate( (pdTASK_CODE)laufschrift_task, 	   "laufschrift",     configMINIMAL_STACK_SIZE, 0, TASK_PRIORITY, NULL);

	/*xTaskCreate( (pdTASK_CODE)lcd_balkenanzeige, 	"lcd", 256, 0, TASK_PRIORITY, NULL);

	xTaskCreate( (pdTASK_CODE)lcd_laufschrift, 	"lcd", 256, 0, TASK_PRIORITY, NULL);

	// Displaying strings in a tasks only works when doing all of this before
	std::stringstream output;
	std::string outputstring;
	const char * chararray;
	output.str(std::string());
	output << "Counter: " << 0;
	outputstring = "";
	outputstring = output.str();
	chararray = "";
	chararray = outputstring.c_str();

	xTaskCreate( (pdTASK_CODE)lcd_zeit, 	"lcd", 256, 0, TASK_PRIORITY, NULL);*/

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
