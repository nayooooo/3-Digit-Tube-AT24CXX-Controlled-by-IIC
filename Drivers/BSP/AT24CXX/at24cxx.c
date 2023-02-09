#include "at24cxx.h"

#include "stdio.h"

#include "iic.h"

/*=================================================
	AT24CXX��BUFF
=================================================*/

uint8_t EE_Write_Buffer[EE_SIZE] = {0};
uint8_t EE_Read_Buffer[EE_SIZE] = {0};

/*=================================================
	AT24CXX��ض�д����
=================================================*/

/**
 * @fn static at24cxx_err_t AT24CXX_Read(uint16_t MemAddress, uint8_t *pData, uint16_t Size)
 * @brief ��ȡAT24CXX�ڴ��е�һ�ֽ�
 *
 * @param [MemAddress] AT24CXX�ڴ��ַ
 * @param [pData] �������ݻ�����ָ��
 * @param [Size] ����������Ŀ
 * @return [at24cxx_err_t] ����ִ��״̬
 *			AT24CXX_OK		->		����ִ�гɹ�
 *			AT24CXX_ERROR	->		����ִ��ʧ��
 *
 */
static at24cxx_err_t AT24CXX_Read(uint16_t MemAddress, uint8_t *pData, uint16_t Size)
{
	if (I2C_OK != I2C2_Read(EE_READ, MemAddress, I2C_MEMADD_SIZE_8BIT, pData, Size))
		return AT24CXX_ERROR;
	return AT24CXX_OK;
}

/**
 * @fn at24cxx_err_t AT24CXX_Read_OneByte(uint16_t MemAddress)
 * @brief ��ȡAT24CXX�ڴ��е�һ�ֽ�
 *
 * @param [MemAddress] AT24CXX�ڴ��ַ
 * @return [uint8_t] ��ȡ�����ֽ�����
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
 * @brief ��ȡAT24CXX�ڴ��������
 *
 * @param [MemAddress] AT24CXX�ڴ��ַ
 * @param [pData] �������ݻ�����ָ��
 * @param [Size] ����������Ŀ
 * @return [at24cxx_err_t] ����ִ��״̬
 *			AT24CXX_OK		->		����ִ�гɹ�
 *			AT24CXX_ERROR	->		����ִ��ʧ��
 *
 */
at24cxx_err_t AT24CXX_Read_MultiByte(uint16_t MemAddress, uint8_t *pData, uint16_t Size)
{
	if ( I2C_OK != AT24CXX_Read(MemAddress, pData, Size))
		return AT24CXX_ERROR;
	
	return AT24CXX_OK;
}

/*=================================================
	AT24CXX��ص��Ժ���
=================================================*/

/**
 * @fn at24cxx_err_t AT24CXX_Print_Read_Buffer(void)
 * @brief ��ӡAT24CXX�Ľ������ݻ���������
 *
 * @return [at24cxx_err_t] ����ִ��״̬
 *			AT24CXX_OK		->		����ִ�гɹ�
 *			AT24CXX_ERROR	->		����ִ��ʧ��
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
	AT24CXX��ʼ��
=================================================*/

/**
 * @fn static at24cxx_err_t AT24C02_I2C_Config(void)
 * @brief ����AT24C02��IIC
 *
 * @return [at24cxx_err_t] ����ִ��״̬
 *			AT24CXX_OK		->		����ִ�гɹ�
 *			AT24CXX_ERROR	->		����ִ��ʧ��
 *
 */
static at24cxx_err_t AT24C02_I2C_Config(void)
{
	if (I2C2_Init() != I2C_OK) return AT24CXX_ERROR;
	
	return AT24CXX_OK;
}

/**
 * @fn static at24cxx_err_t AT24C02_Init(void)
 * @brief ��ʼ��AT24C02
 *
 * @return [at24cxx_err_t] ����ִ��״̬
 *			AT24CXX_OK		->		����ִ�гɹ�
 *			AT24CXX_ERROR	->		����ִ��ʧ��
 *
 */
static at24cxx_err_t AT24C02_Init(void)
{
	if (AT24C02_I2C_Config() != AT24CXX_OK) return AT24CXX_ERROR;
	
	return AT24CXX_OK;
}

/**
 * @fn at24cxx_err_t AT24C02_Init(void)
 * @brief ��ʼ��AT24CXX
 *
 * @return [at24cxx_err_t] ����ִ��״̬
 *			AT24CXX_OK		->		����ִ�гɹ�
 *			AT24CXX_ERROR	->		����ִ��ʧ��
 *
 */
at24cxx_err_t AT24CXX_Init(void)
{
	return AT24C02_Init();
}
