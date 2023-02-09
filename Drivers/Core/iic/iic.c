#include "iic.h"

/*=================================================
	I2C相关变量
=================================================*/

I2C_HandleTypeDef hi2c2;  // IIC2句柄

/*=================================================
	I2C相关读写函数
=================================================*/

/**
 * @fn i2c_err_t I2C2_Read(
 * 	uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize,
 * 	uint8_t *pData, uint16_t Size
 * )
 * @brief 从目标地址读取大量数据
 *
 * @param [DevAddress] 目标器件地址
 * @param [MemAddress] 目标器件内存地址
 * @param [MemAddSize] 目标器件内存单位大小
 * @param [pData] 接收数据缓冲区指针
 * @param [Size] 接收数据数目
 *
 */
i2c_err_t I2C2_Read(
	uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize,
	uint8_t *pData, uint16_t Size
)
{
	if (HAL_OK != HAL_I2C_Mem_Read(&hi2c2, DevAddress, MemAddress, MemAddSize, pData, Size, 10))
		return I2C_ERROR;
	
	return I2C_OK;
}

/*=================================================
	I2C初始化
=================================================*/

i2c_err_t I2C2_Init(void)
{
	hi2c2.Instance = I2C2;
	hi2c2.Init.Timing = 0x00303D5B;
	hi2c2.Init.OwnAddress1 = 0;
	hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c2.Init.OwnAddress2 = 0;
	hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c2) != HAL_OK)
	{
		Error_Handler();
		return I2C_ERROR;
	}
	
	/** Configure Analogue filter
	*/
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	{
		Error_Handler();
		return I2C_ERROR;
	}

	/** Configure Digital filter
	*/
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
	{
		Error_Handler();
		return I2C_ERROR;
	}
	
	return I2C_OK;
}

void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if(hi2c->Instance==I2C2)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
		
		/**I2C2 GPIO Configuration
		PA11 [PA9]	------>	I2C2_SCL
		PA12 [PA10]	------>	I2C2_SDA
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF6_I2C2;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		/* I2C2 clock enable */
		__HAL_RCC_I2C2_CLK_ENABLE();
	}
}
