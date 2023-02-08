#ifndef __SEG_H__
#define __SEG_H__

#include "sys.h"

/*============================================================================
	数码管配置宏
============================================================================*/

#define SEG_CONNECT_NEG						0					/* 共阴极 */
#define SEG_CONNECT_POS						1					/* 共阳极 */
#define SEG_CONNECT_STATE					SEG_CONNECT_NEG

#define SEG_USE_BIT_BAND_OPERATION					/* 使用类位带操作 */

/*============================================================================
	数码管类位带操作
============================================================================*/

#ifdef SEG_USE_BIT_BAND_OPERATION
	typedef struct{
		uint16_t seg_du: 8;
		uint16_t place: 8;
	}SEG_BitBand_t;
	#define SEG_DU	( ((SEG_BitBand_t*)(&(GPIOA->ODR)))->seg_du )
#endif /* SEG_USE_BIT_BAND_OPERATION */

/*============================================================================
	数码管管脚定义
============================================================================*/

#define SEG_A_G_GPIO_PORT				GPIOA
#define SEG_DP_GPIO_PORT				GPIOA
#define SEG_G1_GPIO_PORT				GPIOA
#define SEG_G2_GPIO_PORT				GPIOC
#define SEG_G3_GPIO_PORT				GPIOC
#define SEG_A_G_GPIO_PIN				(GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2\
											|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5\
											|GPIO_PIN_6)
#define SEG_DP_GPIO_PIN					GPIO_PIN_7
#define SEG_G1_GPIO_PIN					GPIO_PIN_8
#define SEG_G2_GPIO_PIN					GPIO_PIN_14
#define SEG_G3_GPIO_PIN					GPIO_PIN_15

#define SEG_A_G_GPIO_CLK_ENABLE()		do {\
											__HAL_RCC_GPIOA_CLK_ENABLE();\
										} while(0)
#define SEG_DP_GPIO_CLK_ENABLE()		do {\
											__HAL_RCC_GPIOA_CLK_ENABLE();\
										} while(0)
#define SEG_G1_GPIO_CLK_ENABLE()		do {\
											__HAL_RCC_GPIOA_CLK_ENABLE();\
										} while(0)
#define SEG_G2_GPIO_CLK_ENABLE()		do {\
											__HAL_RCC_GPIOC_CLK_ENABLE();\
										} while(0)
#define SEG_G3_GPIO_CLK_ENABLE()		do {\
											__HAL_RCC_GPIOC_CLK_ENABLE();\
										} while(0)

#define SEG_A				PAout(0)
#define SEG_B				PAout(1)
#define SEG_C				PAout(2)
#define SEG_D				PAout(3)
#define SEG_E				PAout(4)
#define SEG_F				PAout(5)
#define SEG_G				PAout(6)
#define SEG_DP				PAout(7)
#define SEG_G1				PAout(8)
#define SEG_G2				PCout(14)
#define SEG_G3				PCout(15)
#define SEG(X)				SEG_##X
										
typedef enum{
	SEG_A_G_ON = GPIO_PIN_SET,
	SEG_A_G_OFF = GPIO_PIN_RESET
}SEG_A_G_t;
typedef enum{
	SEG_DP_ON = GPIO_PIN_SET,
	SEG_DP_OFF = GPIO_PIN_RESET
}SEG_DP_t;
typedef enum{
	SEG_Gx_ON = GPIO_PIN_RESET,
	SEG_Gx_OFF = GPIO_PIN_SET
}SEG_Gx_t;

typedef enum{
	SEG_OK = 0,
	SEG_ERROR = 1
}seg_err_t;

/*============================================================================
	数码管相关函数
============================================================================*/

seg_err_t Seg_Init(void);

seg_err_t Seg_Clear(void);

seg_err_t Seg_Show_1_Num(uint8_t x, uint8_t num);
seg_err_t Seg_Show_Num(uint8_t x, uint16_t num, uint8_t len);

#endif /* __SEG_H__ */
