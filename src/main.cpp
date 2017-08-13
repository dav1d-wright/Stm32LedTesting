//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdlib.h>


#include "stm32f7xx_nucleo_144.h"

#include "main.h"
#include <FastLED.h>

static void SystemClock_Config(void);
static void CPU_CACHE_Enable(void);
void Error_Handler(void);

#define NUM_LEDS 60
// ----------------------------------------------------------------------------
//
// Print a greeting message on the trace device and enter a loop
// to count seconds.
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the DEBUG output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//
// ----------------------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

int main (void)
{
	UART_HandleTypeDef displayUartHandle;

	int i = 1;

	HAL_Init();

	CPU_CACHE_Enable();

	SystemClock_Config();


	BSP_LED_Init(LED3);

	displayUartHandle.Instance        = USARTx;

	displayUartHandle.Init.BaudRate   = 115200;
	displayUartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	displayUartHandle.Init.StopBits   = UART_STOPBITS_1;
	displayUartHandle.Init.Parity     = UART_PARITY_EVEN;
	displayUartHandle.Init.HwFlowCtl  = UART_HWCONTROL_RTS_CTS;
	displayUartHandle.Init.Mode       = UART_MODE_TX_RX;
	displayUartHandle.gState = HAL_UART_STATE_RESET;

	if(HAL_UART_Init(&displayUartHandle) != HAL_OK)
	{
		Error_Handler();
	}

	// disable inverse mode
//	char ycMsg[512];
//	sprintf(ycMsg, "\x1B[?3l");
//	if(HAL_UART_Transmit(&displayUartHandle, (uint8_t*)ycMsg, sizeof(ycMsg)/sizeof(uint8_t), 1000) != HAL_OK)
//	{
//		Error_Handler();
//	}

//	// enable automatic line wrap
//	sprintf(ycMsg, "\x1B[?8h");
//	if(HAL_UART_Transmit(&displayUartHandle, (uint8_t*)ycMsg, sizeof(ycMsg)/sizeof(uint8_t), 1000) != HAL_OK)
//	{
//		Error_Handler();
//	}

	// clear display
//	sprintf(ycMsg, "\x1B[J");
//	if(HAL_UART_Transmit(&displayUartHandle, (uint8_t*)ycMsg, sizeof(ycMsg)/sizeof(uint8_t), 1000) != HAL_OK)
//	{
//		Error_Handler();
//	}

//	// go to (0,0)
//	sprintf(ycMsg, "\x1B[0;0H");
//	if(HAL_UART_Transmit(&displayUartHandle, (uint8_t*)ycMsg, sizeof(ycMsg)/sizeof(uint8_t), 1000) != HAL_OK)
//	{
//		Error_Handler();
//	}


	uint8_t yuHello = (uint8_t)'c';
	if(HAL_UART_Transmit(&displayUartHandle, &yuHello, sizeof(yuHello)/sizeof(uint8_t), 1000) != HAL_OK)
	{
		Error_Handler();
	}

	CRGB leds[NUM_LEDS];
//	// Port C, Pin 8
	FastLED.addLeds<NEOPIXEL, 40>(leds, NUM_LEDS);

//	// Send a greeting to the trace device (skipped on Release).
//	trace_puts("Hello ARM World!");
//
//	// At this stage the system clock should have already been configured
//	// at high speed.
//	trace_printf("System clock: %u Hz\n", SystemCoreClock);


	// Infinite loop
	while (1)
	{
		i++;
//		leds[0] = CRGB::White; FastLED.show(); delay(30);
//		leds[0] = CRGB::Black; FastLED.show(); delay(30);
	}
	// Infinite loop, never return.
}

static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}

void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 432;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  RCC_OscInitStruct.PLL.PLLR = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    while(1) {};
  }

  /* Activate the OverDrive to reach the 216 Mhz Frequency */
  if(HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    while(1) {};
  }


  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    while(1) {};
  }

  // update system clock value
  SystemCoreClockUpdate();
}

void Error_Handler(void)
{
	BSP_LED_On(LED3);

	while(true);
}
#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
