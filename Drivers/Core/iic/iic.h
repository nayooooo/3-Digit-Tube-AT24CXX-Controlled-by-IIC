#ifndef __IIC_H__
#define __IIC_H__

#include "sys.h"

/*=================================================
	I2C函数执行状态
=================================================*/

typedef enum{
	I2C_OK = 0,
	I2C_ERROR = 1
}i2c_err_t;

/*=================================================
	I2C相关变量
=================================================*/

extern I2C_HandleTypeDef hi2c2;  // IIC2句柄

/*=================================================
	I2C相关函数
=================================================*/

i2c_err_t I2C2_Init(void);

i2c_err_t I2C2_Read(
	uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize,
	uint8_t *pData, uint16_t Size
);

#endif /* __IIC_H__ */
