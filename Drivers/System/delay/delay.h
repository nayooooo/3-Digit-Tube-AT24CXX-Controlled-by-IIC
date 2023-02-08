#ifndef __DELAY_H
#define __DELAY_H

#include "sys.h"

void delay_Init(uint8_t SYSCLK_M);

void delay_us(uint32_t nus);
void delay_ms(uint32_t nms);

#endif /* __DELAY_H */
