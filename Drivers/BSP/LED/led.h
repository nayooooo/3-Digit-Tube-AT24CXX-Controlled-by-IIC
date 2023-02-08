#ifndef __LED_H
#define __LED_H

#include "sys.h"

#define LED_GPIO_PORT				GPIOB
#define LED_GPIO_PIN				GPIO_PIN_7
#define LED_GPIO_CLK_ENABLE()		do {\
										__HAL_RCC_GPIOB_CLK_ENABLE();\
									} while(0)

typedef enum{
	LED_ON = GPIO_PIN_SET,
	LED_OFF = GPIO_PIN_RESET
}LED_State_Enum_t;
#define LED			PBout(7)

void LED_Init(void);

GPIO_PinState LED_Read(void);
void LED_Toggle(void);

#endif /* __LED_H */
