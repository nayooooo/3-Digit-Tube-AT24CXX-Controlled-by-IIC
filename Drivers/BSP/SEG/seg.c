#include "seg.h"

#include "delay.h"

/*============================================================================
	数码管数码表
============================================================================*/

#if SEG_CONNECT_STATE == SEG_CONNECT_NEG  // 共阴极数码管
	const unsigned char Seg_Num_Table[] = {
	//		0	1	 2	  3	   4	5	 6	  7	   8	9
		0X3F,0X06,0X5B,0X4F,0X66,0X6D,0X7D,0X07,0X7F,0X6F,
	};
#elif SEG_CONNECT_STATE == SEG_CONNECT_POS  // 共阳极数码管
	const unsigned char Seg_Num_Table[] = {};
#endif /* SEG_CONNECT_STATE */

/*============================================================================
	数学运算
============================================================================*/

static uint32_t my_pow(uint32_t m, uint32_t n)
{
	uint32_t res = 1;
	
	while (n--) {
		res *= m;
	}
	
	return res;
}

/*============================================================================
	数码管显示函数
============================================================================*/

/**
 * @fn seg_err_t Seg_Clear(void)
 * @brief 数码管清除显示
 *
 * @return [seg_err_t] 函数执行状态
 *			SEG_OK		->		函数执行成功
 *			SEG_ERROR	->		函数执行失败
 *
 */
seg_err_t Seg_Clear(void)
{
	SEG_G1 = SEG_Gx_OFF;
	SEG_G2 = SEG_Gx_OFF;
	SEG_G3 = SEG_Gx_OFF;
	SEG_A = SEG_A_G_OFF;
	SEG_B = SEG_A_G_OFF;
	SEG_C = SEG_A_G_OFF;
	SEG_D = SEG_A_G_OFF;
	SEG_E = SEG_A_G_OFF;
	SEG_F = SEG_A_G_OFF;
	SEG_G = SEG_A_G_OFF;
	SEG_DP = SEG_A_G_OFF;
	
	return SEG_OK;
}

/**
 * @fn seg_err_t Seg_Show_1_Num(uint8_t x, uint8_t num, uint8_t len)
 * @brief 数码管显示一位数字
 *
 * @param [x] 显示的起势位置
 * @param [num] 待显示的数字
 * @return [seg_err_t] 函数执行状态
 *			SEG_OK		->		函数执行成功
 *			SEG_ERROR	->		函数执行失败
 *
 */
seg_err_t Seg_Show_1_Num(uint8_t x, uint8_t num)
{
	if (x >= 3) return SEG_ERROR;
	if (num >= 10) return SEG_ERROR;
	
	switch (x)
	{
		case 0:
			SEG_G1 = SEG_Gx_ON;
			break;
		case 1:
			SEG_G2 = SEG_Gx_ON;
			break;
		case 2:
			SEG_G3 = SEG_Gx_ON;
			break;
	}
	
#ifdef SEG_USE_BIT_BAND_OPERATION
	SEG_DU = Seg_Num_Table[num];
#else
	uint8_t temp = Seg_Num_Table[num];
	if (temp&0X01) SEG_A = SEG_A_G_ON;
	else SEG_A = SEG_A_G_OFF;
	temp >>= 1;
	if (temp&0X01) SEG_B = SEG_A_G_ON;
	else SEG_B = SEG_A_G_OFF;
	temp >>= 1;
	if (temp&0X01) SEG_C = SEG_A_G_ON;
	else SEG_C = SEG_A_G_OFF;
	temp >>= 1;
	if (temp&0X01) SEG_D = SEG_A_G_ON;
	else SEG_D = SEG_A_G_OFF;
	temp >>= 1;
	if (temp&0X01) SEG_E = SEG_A_G_ON;
	else SEG_E = SEG_A_G_OFF;
	temp >>= 1;
	if (temp&0X01) SEG_F = SEG_A_G_ON;
	else SEG_F = SEG_A_G_OFF;
	temp >>= 1;
	if (temp&0X01) SEG_G = SEG_A_G_ON;
	else SEG_G = SEG_A_G_OFF;
	temp >>= 1;
	if (temp&0X01) SEG_DP = SEG_A_G_ON;
	else SEG_DP = SEG_A_G_OFF;
#endif
	
	return SEG_OK;
}

/**
 * @fn seg_err_t Seg_Show_Num(uint8_t x, uint16_t num, uint8_t len)
 * @brief 数码管显示数字
 *
 * @param [x] 显示的起势位置
 * @param [num] 待显示的数字
 * @param [len] 数字长度（位数）
 * @return [seg_err_t] 函数执行状态
 *			SEG_OK		->		函数执行成功
 *			SEG_ERROR	->		函数执行失败
 *
 */
seg_err_t Seg_Show_Num(uint8_t x, uint16_t num, uint8_t len)
{
	uint8_t i;
	uint8_t num_temp;
	
	if (x >= 3) return SEG_ERROR;
	if (num >= 1000) return SEG_ERROR;
	if (len > 3) return SEG_ERROR;
	
	for (i = 0; i < len; i++) {
		if (x + i >= 3) return SEG_ERROR;
		// 取得位数为(len - i)的数
		// 然后获取该数的十进制最高位
		num_temp = num % my_pow(10, len - i) / my_pow(10, len - i - 1);
		Seg_Clear();
		Seg_Show_1_Num(x + i, num_temp);
		delay_ms(1);
	}
	
	return SEG_OK;
}

/*============================================================================
	数码管初始化
============================================================================*/

static seg_err_t Seg_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	
	SEG_A_G_GPIO_CLK_ENABLE();
	GPIO_InitStruct.Pin = SEG_A_G_GPIO_PIN;
	HAL_GPIO_Init(SEG_A_G_GPIO_PORT, &GPIO_InitStruct);
	
	SEG_DP_GPIO_CLK_ENABLE();
	GPIO_InitStruct.Pin = SEG_DP_GPIO_PIN;
	HAL_GPIO_Init(SEG_DP_GPIO_PORT, &GPIO_InitStruct);
	
	SEG_G1_GPIO_CLK_ENABLE();
	GPIO_InitStruct.Pin = SEG_G1_GPIO_PIN;
	HAL_GPIO_Init(SEG_G1_GPIO_PORT, &GPIO_InitStruct);
	
	SEG_G2_GPIO_CLK_ENABLE();
	GPIO_InitStruct.Pin = SEG_G2_GPIO_PIN;
	HAL_GPIO_Init(SEG_G2_GPIO_PORT, &GPIO_InitStruct);
	
	SEG_G3_GPIO_CLK_ENABLE();
	GPIO_InitStruct.Pin = SEG_G3_GPIO_PIN;
	HAL_GPIO_Init(SEG_G3_GPIO_PORT, &GPIO_InitStruct);
	
	return SEG_OK;
}

static seg_err_t Seg_Map_Init(void)
{
	return Seg_Clear();
}

/**
 * @fn void Seg_Init(void)
 * @brief 初始化数码管
 *
 */
seg_err_t Seg_Init(void)
{
	uint8_t err_times = 0;
	
	while((err_times < 50) && (Seg_GPIO_Config() != SEG_OK)) err_times++;
	if(!(err_times < 50)) return SEG_ERROR;
	while((err_times < 50) && (Seg_Map_Init() != SEG_OK)) err_times++;
	if(!(err_times < 50)) return SEG_ERROR;
	
	return SEG_OK;
}
