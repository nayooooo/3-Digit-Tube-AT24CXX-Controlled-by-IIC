#include "at24cxx.h"

#include "stdio.h"
#include "delay.h"

#include "iic.h"

/*=================================================
	AT24CXX的BUFF
=================================================*/

uint8_t EE_Write_Buffer[EE_SIZE] = {0};
uint8_t EE_Read_Buffer[EE_SIZE] = {0};

/*=================================================
	AT24CXX相关读写函数
=================================================*/

/**
 * @fn static at24cxx_err_t AT24CXX_Read(uint16_t MemAddress, uint8_t *pData, uint16_t Size)
 * @brief 读取AT24CXX内存中的任意数目字节
 *
 * @param [MemAddress] AT24CXX内存地址
 * @param [pData] 接收数据缓冲区指针
 * @param [Size] 接收数据数目
 * @return [at24cxx_err_t] 函数执行状态
 *			AT24CXX_OK		->		函数执行成功
 *			AT24CXX_ERROR	->		函数执行失败
 *
 */
static at24cxx_err_t AT24CXX_Read(uint16_t MemAddress, uint8_t *pData, uint16_t Size)
{
	if (MemAddress + Size > EE_SIZE) return AT24CXX_ERROR;
	
	if (I2C_OK != I2C2_Read(EE_READ, MemAddress, I2C_MEMADD_SIZE_8BIT, pData, Size))
		return AT24CXX_ERROR;
	return AT24CXX_OK;
}

/**
 * @fn uint8_t AT24CXX_Read_OneByte(uint16_t MemAddress)
 * @brief 读取AT24CXX内存中的一字节
 *
 * @param [MemAddress] AT24CXX内存地址
 * @return [uint8_t] 读取到的字节数据
 *
 */
uint8_t AT24CXX_Read_OneByte(uint16_t MemAddress)
{
	uint8_t rec_chr = 0X00;
	
	if ( I2C_OK != AT24CXX_Read(MemAddress, &rec_chr, 1))
		return NULL;
	
	return rec_chr;
}

/**
 * @fn at24cxx_err_t AT24CXX_Read_MultiByte(uint16_t MemAddress, uint8_t *pData, uint16_t Size)
 * @brief 读取AT24CXX内存大量内容
 *
 * @param [MemAddress] AT24CXX内存地址
 * @param [pData] 接收数据缓冲区指针
 * @param [Size] 接收数据数目
 * @return [at24cxx_err_t] 函数执行状态
 *			AT24CXX_OK		->		函数执行成功
 *			AT24CXX_ERROR	->		函数执行失败
 *
 */
at24cxx_err_t AT24CXX_Read_MultiByte(uint16_t MemAddress, uint8_t *pData, uint16_t Size)
{
	return AT24CXX_Read(MemAddress, pData, Size);
}

/**
 * @fn
 * @brief 从AT24CXX读出长度为len字节的数据
 *
 * @param [read_data] 所读出数据需写入的变量的指针
 * @param [len] 数据的长度
 * @return [at24cxx_err_t] 函数执行状态
 *			AT24CXX_OK		->		函数执行成功
 *			AT24CXX_ERROR	->		函数执行失败
 *
 */
at24cxx_err_t AT24CXX_Read_LenByte(void *read_data, uint8_t len)
{
	UNUSED(read_data);
	UNUSED(len);
	
	return AT24CXX_OK;
}

/**
 * @fn static at24cxx_err_t AT24CXX_Write(uint16_t MemAddress, uint8_t *pData, uint16_t Size)
 * @brief 向AT24CXX内存中写入任意数目字节
 *
 * @param [MemAddress] AT24CXX内存地址
 * @param [pData] 写入数据缓冲区指针
 * @param [Size] 写入数据数目
 * @return [at24cxx_err_t] 函数执行状态
 *			AT24CXX_OK		->		函数执行成功
 *			AT24CXX_ERROR	->		函数执行失败
 *
 */
static at24cxx_err_t AT24CXX_Write(uint16_t MemAddress, uint8_t *pData, uint16_t Size)
{
	if (MemAddress + Size > EE_SIZE) return AT24CXX_ERROR;
	
	if (I2C_OK != I2C2_Write(EE_READ, MemAddress, I2C_MEMADD_SIZE_8BIT, pData, Size))
		return AT24CXX_ERROR;
	return AT24CXX_OK;
}

/**
 * @fn at24cxx_err_t AT24CXX_Write_OneByte(uint16_t MemAddress, uint8_t write_byte)
 * @brief 向AT24CXX内存中写入一字节
 *
 * @param [MemAddress] AT24CXX内存地址
 * @param [write_byte] 待写入的字节
 * @return [at24cxx_err_t] 函数执行状态
 *			AT24CXX_OK		->		函数执行成功
 *			AT24CXX_ERROR	->		函数执行失败
 *
 */
at24cxx_err_t AT24CXX_Write_OneByte(uint16_t MemAddress, uint8_t write_byte)
{
	at24cxx_err_t ret;
	
	ret = AT24CXX_Write(MemAddress, &write_byte, 1);
	delay_ms(5);  // 写周期，最大5ms
	
	return ret;
}

/**
 * @fn at24cxx_err_t AT24CXX_Write_MultiByte(uint16_t MemAddress, uint8_t write_byte)
 * @brief 向AT24CXX内存中写入大量内容
 *
 * @param [MemAddress] AT24CXX内存地址
 * @param [pData] 写入数据缓冲区指针
 * @param [Size] 写入数据数目
 * @return [at24cxx_err_t] 函数执行状态
 *			AT24CXX_OK		->		函数执行成功
 *			AT24CXX_ERROR	->		函数执行失败
 *
 */
at24cxx_err_t AT24CXX_Write_MultiByte(uint16_t MemAddress, uint8_t *pData, uint16_t Size)
{
	return AT24CXX_Write(MemAddress, pData, Size);
}

/*=================================================
	AT24CXX相关调试函数
=================================================*/

/**
 * @fn at24cxx_err_t AT24CXX_Print_Read_Buffer(void)
 * @brief 打印AT24CXX的接收数据缓冲区内容
 *
 * @return [at24cxx_err_t] 函数执行状态
 *			AT24CXX_OK		->		函数执行成功
 *			AT24CXX_ERROR	->		函数执行失败
 *
 */
at24cxx_err_t AT24CXX_Print_Read_Buffer(void)
{
	uint16_t i;
	uint8_t j;
	
	printf("\r\n");
	printf("===================================================================\r\n");
	printf("                           AT24C02 start\r\n");
	printf("===================================================================\r\n");
	for (i = 0; i < EE_SIZE / EE_PAGE_SIZE; i++) {
		printf("%u:\t", i);
		for (j = 0; j < EE_PAGE_SIZE; j++) {
			printf("0X%X\t", EE_Read_Buffer[i * EE_PAGE_SIZE + j]);
		}
		printf("\r\n");
	}
	printf("===================================================================\r\n");
	printf("                            AT24C02 end\r\n");
	printf("===================================================================\r\n");
	printf("\r\n");
	
	return AT24CXX_OK;
}

/*=================================================
	AT24CXX初始化
=================================================*/

/**
 * @fn static at24cxx_err_t AT24C02_I2C_Config(void)
 * @brief 配置AT24C02的IIC
 *
 * @return [at24cxx_err_t] 函数执行状态
 *			AT24CXX_OK		->		函数执行成功
 *			AT24CXX_ERROR	->		函数执行失败
 *
 */
static at24cxx_err_t AT24C02_I2C_Config(void)
{
	if (I2C2_Init() != I2C_OK) return AT24CXX_ERROR;
	
	return AT24CXX_OK;
}

/**
 * @fn static at24cxx_err_t AT24C02_Init(void)
 * @brief 初始化AT24C02
 *
 * @return [at24cxx_err_t] 函数执行状态
 *			AT24CXX_OK		->		函数执行成功
 *			AT24CXX_ERROR	->		函数执行失败
 *
 */
static at24cxx_err_t AT24C02_Init(void)
{
	if (AT24C02_I2C_Config() != AT24CXX_OK) return AT24CXX_ERROR;
	
	return AT24CXX_OK;
}

/**
 * @fn at24cxx_err_t AT24C02_Init(void)
 * @brief 初始化AT24CXX
 *
 * @return [at24cxx_err_t] 函数执行状态
 *			AT24CXX_OK		->		函数执行成功
 *			AT24CXX_ERROR	->		函数执行失败
 *
 */
at24cxx_err_t AT24CXX_Init(void)
{
	return AT24C02_Init();
}
