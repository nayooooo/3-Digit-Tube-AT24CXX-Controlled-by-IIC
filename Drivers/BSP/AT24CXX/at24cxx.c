#include "at24cxx.h"

#include "iic.h"

static at24cxx_err_t AT24CXX_IIC_Config(void)
{
	if (IIC_Init() != IIC_OK) return AT24CXX_ERROR;
	
	return AT24CXX_OK;
}

at24cxx_err_t AT24C02_Init(void)
{
	if (AT24CXX_IIC_Config() != AT24CXX_OK) return AT24CXX_ERROR;
	
	return AT24CXX_OK;
}
