#ifndef __IIC_H__
#define __IIC_H__

#include "sys.h"

typedef enum{
	IIC_OK = 0,
	IIC_ERROR = 1
}iic_err_t;

iic_err_t IIC_Init(void);

#endif /* __IIC_H__ */
