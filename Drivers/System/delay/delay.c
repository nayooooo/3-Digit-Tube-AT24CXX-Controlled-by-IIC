#include "delay.h"

static uint32_t fac_us = 0;					// us延时倍数

/**
 * @fn void delay_us(uint32_t nus)
 * @brief 延时n微妙，最大可延时(2^32-1)us
 *
 * @param [nus] 延时微妙数
 *
 */
void delay_us(uint32_t nus)
{
	uint32_t ticks;
	uint32_t told, tnow, tcnt = 0, tcntold = tcnt;
	uint32_t reload = SysTick->LOAD;		// LOAD的值
	ticks = nus * fac_us;					// 需要的节拍数
	told = SysTick->VAL;					// 刚进入时的计数器值
	while (1)
	{
		tnow = SysTick->VAL;
		if (tnow != told) {
			// SysTick递减
			if (tnow < told) tcnt += told - tnow;
			else tcnt += reload - tnow + told;
			told = tnow;
			if (tcntold > tcnt) break;		// 不明原因导致tcnt溢出，跳出阻塞状态
			else tcntold = tcnt;
			if (tcnt >= ticks) break;		// 达到或超过所需节拍数则跳出循环
		}
	}
}

/**
 * @fn void delay_ms(uint32_t nms)
 * @brief 延时n毫妙，最大可延时(2^32-1)ms
 * @details 数 2147483 来自于 (2^32-1)/(16/8(分频数))/1000 的整数部分
 *			不考虑延时时间是因为delay_us函数设计时不需要考虑是否重装载
 *
 * @param [nus] 延时毫妙数
 *
 */
void delay_ms(uint32_t nms)
{
	if (nms < 2147483UL)
		delay_us(nms * 1000);
	else {
		// 剩余延时部分大于 2147483 ms
		while (nms > 2147483UL) {
			delay_us(2147483UL * 1000);
			nms -= 2147483UL;  // 减去已延时的部分
		}
		// 剩余延时部分小于 2147483 ms
		delay_us(nms * 1000);
	}
}

void delay_Init(uint8_t SYSCLK_M)
{
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK_DIV8);
	fac_us = SYSCLK_M / 8;
}
