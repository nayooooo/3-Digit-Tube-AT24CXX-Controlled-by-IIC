#ifndef __SYS_H
#define __SYS_H

/**
 * @file sys.h
 * @author yewan
 * @encode GB2312
 *
 */

#include "stm32g0xx.h"

#define MAIN_Fosc				(16000000L)
#define MAIN_Fosc_M				(MAIN_Fosc / 1000000L)

// 1	->	启用操作系统
// 0	->	禁用操作系统
#define SYSTEM_SUPPORT_OS			(0)

/* 常用类型名重定义 -------------------------------------------*/

typedef uint8_t			u8;
typedef uint16_t		u16;
typedef uint32_t		u32;

#ifndef UNUSED
	#define UNUSED(X) (void)X      /* To avoid gcc/g++ warnings */
#endif /* UNUSED(X) */

/* 类位带操作 -------------------------------------------------*/

// 输入
#define PAin(n)		( ((GPIOA->IDR)&(1<<(n)))>>(n) )
#define PBin(n)		( ((GPIOB->IDR)&(1<<(n)))>>(n) )
#define PCin(n)		( ((GPIOC->IDR)&(1<<(n)))>>(n) )
#define PDin(n)		( ((GPIOD->IDR)&(1<<(n)))>>(n) )
#define PEin(n)		( ((GPIOE->IDR)&(1<<(n)))>>(n) )
#define PFin(n)		( ((GPIOF->IDR)&(1<<(n)))>>(n) )

// 输出
/* bit-band struct */
typedef struct{  // These bits can be read and written by software.
	uint16_t bit0: 1;
	uint16_t bit1: 1;
	uint16_t bit2: 1;
	uint16_t bit3: 1;
	uint16_t bit4: 1;
	uint16_t bit5: 1;
	uint16_t bit6: 1;
	uint16_t bit7: 1;
	uint16_t bit8: 1;
	uint16_t bit9: 1;
	uint16_t bit10: 1;
	uint16_t bit11: 1;
	uint16_t bit12: 1;
	uint16_t bit13: 1;
	uint16_t bit14: 1;
	uint16_t bit15: 1;
} Bits16_TypeDef;
#define PAout(n)	( ((Bits16_TypeDef*)(&(GPIOA->ODR)))->bit##n )
#define PBout(n)	( ((Bits16_TypeDef*)(&(GPIOB->ODR)))->bit##n )
#define PCout(n)	( ((Bits16_TypeDef*)(&(GPIOC->ODR)))->bit##n )
#define PDout(n)	( ((Bits16_TypeDef*)(&(GPIOD->ODR)))->bit##n )
#define PEout(n)	( ((Bits16_TypeDef*)(&(GPIOE->ODR)))->bit##n )
#define PFout(n)	( ((Bits16_TypeDef*)(&(GPIOF->ODR)))->bit##n )

// 初始化时钟树
void sys_stm32_clock_init(void);

void sys_wfi_set(void);
void sys_standby(void);
void sys_soft_reset(void);

void Error_Handler(void);
#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line);
#endif /* USE_FULL_ASSERT */

#endif /* __SYS_H */
