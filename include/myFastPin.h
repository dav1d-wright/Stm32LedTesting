#include "stm32f7xx_hal_gpio.h"

#define OUTPUT GPIO_MODE_OUTPUT_PP
#define INPUT GPIO_MODE_INPUT

GPIO_TypeDef* digitalPinToPort(uint8_t auPin)
{
	GPIO_TypeDef* ptPort = 0;
	if((auPin >= 0) && (auPin <= 15))
	{
		ptPort = GPIOA;
	}
	else if((auPin >= 16) && (auPin <= 31))
	{
		ptPort = GPIOB;
	}
	else if((auPin >= 32) && (auPin <= 47))
	{
		ptPort = GPIOC;
	}
	else if((auPin >= 48) && (auPin <= 63))
	{
		ptPort = GPIOD;
	}
	else if((auPin >= 64) && (auPin <= 79))
	{
		ptPort = GPIOE;
	}
	else if((auPin >= 80) && (auPin <= 95))
	{
		ptPort = GPIOF;
	}
	else if((auPin >= 96) && (auPin <= 111))
	{
		ptPort = GPIOG;
	}
	else if((auPin >= 112) && (auPin <= 127))
	{
		ptPort = GPIOH;
	}
	else if((auPin >= 128) && (auPin <= 143))
	{
		ptPort = GPIOI;
	}
	else if((auPin >= 144) && (auPin <= 159))
	{
		ptPort = GPIOJ;
	}
	else if((auPin >= 160) && (auPin <= 167))
	{
		ptPort = GPIOK;
	}

	return ptPort;
}

volatile uint32_t* portOutputRegister(GPIO_TypeDef* aptGpioPort)
{
	volatile uint32_t* puOutReg = &(aptGpioPort->ODR);
	return puOutReg;
}

volatile uint32_t* portInputRegister(GPIO_TypeDef* aptGpioPort)
{
	volatile uint32_t* puInReg = &(aptGpioPort->IDR);
	return puInReg;
}

uint32_t digitalPinToBitMask(uint8_t auDigitalPin)
{
	uint8_t uPin = auDigitalPin % 16U;
	uint32_t uMask = 1U << uPin;
	return uMask;
}

inline static void pinMode(uint8_t auPin, uint32_t auMode)
{
	uint32_t uGpioPin = 1U << auPin;

	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_TypeDef* ptGpioPort = digitalPinToPort(auPin);

  __HAL_RCC_GPIOB_CLK_ENABLE();

  GPIO_InitStruct.Mode  = auMode;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

  GPIO_InitStruct.Pin = uGpioPin;

  HAL_GPIO_Init(ptGpioPort, &GPIO_InitStruct);
}
