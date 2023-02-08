#include "led.h"

static void LED_GPIO_Config(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	LED_GPIO_CLK_ENABLE();
	
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void LED_Init(void)
{
	LED_GPIO_Config(LED_GPIO_PORT, LED_GPIO_PIN);
	
	LED = LED_OFF;
}

GPIO_PinState LED_Read(void)
{
	return HAL_GPIO_ReadPin(LED_GPIO_PORT, LED_GPIO_PIN);
}

void LED_Toggle(void)
{
	HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_GPIO_PIN);
}
