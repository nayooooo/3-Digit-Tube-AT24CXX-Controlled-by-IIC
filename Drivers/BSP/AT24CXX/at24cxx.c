#include "at24cxx.h"

#include "stdio.h"
#include "delay.h"

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
 * @brief ��ȡAT24CXX�ڴ��е�������Ŀ�ֽ�
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
	if (MemAddress + Size > EE_SIZE) return AT24CXX_ERROR;
	
	if (I2C_OK != I2C2_Read(EE_READ, MemAddress, I2C_MEMADD_SIZE_8BIT, pData, Size))
		return AT24CXX_ERROR;
	return AT24CXX_OK;
}

/**
 * @fn uint8_t AT24CXX_Read_OneByte(uint16_t MemAddress)
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
	return AT24CXX_Read(MemAddress, pData, Size);
}

/**
 * @fn
 * @brief ��AT24CXX��������Ϊlen�ֽڵ�����
 *
 * @param [read_data] ������������д��ı�����ָ��
 * @param [len] ���ݵĳ���
 * @return [at24cxx_err_t] ����ִ��״̬
 *			AT24CXX_OK		->		����ִ�гɹ�
 *			AT24CXX_ERROR	->		����ִ��ʧ��
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
 * @brief ��AT24CXX�ڴ���д��������Ŀ�ֽ�
 *
 * @param [MemAddress] AT24CXX�ڴ��ַ
 * @param [pData] д�����ݻ�����ָ��
 * @param [Size] д��������Ŀ
 * @return [at24cxx_err_t] ����ִ��״̬
 *			AT24CXX_OK		->		����ִ�гɹ�
 *			AT24CXX_ERROR	->		����ִ��ʧ��
 *
 */
static at24cxx_err_t AT24CXX_Write(uint16_t MemAddress, uint8_t *pData, uint16_t Size)
{
	if (MemAddress + Size > EE_SIZE) return AT24CXX_ERROR;  // ����Խ��
	if (MemAddress % EE_PAGE_SIZE + Size > EE_PAGE_SIZE) return AT24CXX_ERROR;  // ҳԽ��
	
	if (I2C_OK != I2C2_Write(EE_READ, MemAddress, I2C_MEMADD_SIZE_8BIT, pData, Size))
		return AT24CXX_ERROR;
	return AT24CXX_OK;
}

/**
 * @fn at24cxx_err_t AT24CXX_Write_OneByte(uint16_t MemAddress, uint8_t write_byte)
 * @brief ��AT24CXX�ڴ���д��һ�ֽ�
 *
 * @param [MemAddress] AT24CXX�ڴ��ַ
 * @param [write_byte] ��д����ֽ�
 * @return [at24cxx_err_t] ����ִ��״̬
 *			AT24CXX_OK		->		����ִ�гɹ�
 *			AT24CXX_ERROR	->		����ִ��ʧ��
 *
 */
at24cxx_err_t AT24CXX_Write_OneByte(uint16_t MemAddress, uint8_t write_byte)
{
	at24cxx_err_t ret;
	
	ret = AT24CXX_Write(MemAddress, &write_byte, 1);
	delay_ms(5);  // д���ڣ����5ms
	
	return ret;
}

/**
 * @fn at24cxx_err_t AT24CXX_Write_MultiByte(uint16_t MemAddress, uint8_t write_byte)
 * @brief ��AT24CXX�ڴ���д���������
 *
 * @param [MemAddress] AT24CXX�ڴ��ַ
 * @param [pData] д�����ݻ�����ָ��
 * @param [Size] д��������Ŀ
 * @return [at24cxx_err_t] ����ִ��״̬
 *			AT24CXX_OK		->		����ִ�гɹ�
 *			AT24CXX_ERROR	->		����ִ��ʧ��
 *
 */
at24cxx_err_t AT24CXX_Write_MultiByte(uint16_t MemAddress, uint8_t *pData, uint16_t Size)
{
#if AT24CXX_WRITE_MODE == AT24CXX_WRITE_BYTE_MODE
	
	uint8_t write_byte_num;  // ��д���ֽ���
	
	for (write_byte_num = 0; write_byte_num < Size; write_byte_num++) {
		if (AT24CXX_OK != AT24CXX_Write_OneByte(MemAddress + write_byte_num, *(pData + write_byte_num)))
			return AT24CXX_ERROR;
	}
	// ��AT24CXX_Write_OneByte�������Ѿ���д������
	
#elif AT24CXX_WRITE_MODE == AT24CXX_WRITE_PAGE_MODE
	
	uint8_t page_num;  // �ܹ���Ҫд���ҳ��
	uint16_t cwbn;  // current_write_byte_num ��ǰ��Ҫд����ֽ���
	uint16_t write_byte_num = 0;  // ��д���ֽ���
	
	if (MemAddress % EE_PAGE_SIZE + Size > EE_PAGE_SIZE) {  // ����Խ������
		// ������Ҫ���͵�ҳ�����ֽ���
		page_num = Size / EE_PAGE_SIZE;
		if ((Size + MemAddress) % EE_PAGE_SIZE) {  // ĩβ���ڲ�����ҳ
			page_num += 2;
		} else {  // ĩβ�����ڲ�����ҳ
			page_num += 1;
		}
		cwbn = EE_PAGE_SIZE - MemAddress % EE_PAGE_SIZE;
		// ��ʼд������ֽ�
		while (page_num) {
			// д��һҳ����
			if (AT24CXX_OK != AT24CXX_Write(MemAddress + write_byte_num, pData + write_byte_num, EE_PAGE_SIZE))
				return AT24CXX_ERROR;
			// д����һҳ���ݣ�׼��д����һҳ����
			page_num--;
			write_byte_num += cwbn;
			// д�����һҳʱ�������ǲ�����ҳ
			if ((page_num == 1) && ((Size + MemAddress) % EE_PAGE_SIZE)) {
				cwbn = (Size + MemAddress) % EE_PAGE_SIZE;
			} else cwbn = EE_PAGE_SIZE;
		}
	} else {  // ������Խ������
		if (AT24CXX_OK != AT24CXX_Write(MemAddress, pData, Size))
			return AT24CXX_ERROR;
	}

	delay_ms(5);  // д���ڣ����5ms
	
#endif /* AT24CXX_WRITE_MODE */
	
	return AT24CXX_OK;
}

/*=================================================
	AT24CXX��ص��Ժ���
=================================================*/

/**
 * @fn static at24cxx_err_t AT24CXX_Print_Buffer(uint8_t *pData, uint16_t Size, uint16_t page_Size)
 * @brief ��ӡ���ݻ���������
 *
 * @param [pData] ���ݻ�����ָ��
 * @param [Size] ���ݻ�������С
 * @param [page_Size] ���ݻ�����ҳ��С
 * @return [at24cxx_err_t] ����ִ��״̬
 *			AT24CXX_OK		->		����ִ�гɹ�
 *			AT24CXX_ERROR	->		����ִ��ʧ��
 *
 */
static at24cxx_err_t AT24CXX_Print_Buffer(uint8_t *pData, uint16_t Size, uint16_t page_Size)
{
	uint16_t i;
	uint8_t j;
	
	printf("\r\n");
	printf("===================================================================\r\n");
	printf("                           AT24C02 start\r\n");
	printf("===================================================================\r\n");
	for (i = 0; i < Size / page_Size; i++) {
		printf("%u:\t", i);
		for (j = 0; j < page_Size; j++) {
			printf("0X%X\t", pData[i * page_Size + j]);
		}
		printf("\r\n");
	}
	printf("===================================================================\r\n");
	printf("                            AT24C02 end\r\n");
	printf("===================================================================\r\n");
	printf("\r\n");
	
	return AT24CXX_OK;
}

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
	printf("\r\n");
	printf("===================================================================\r\n");
	printf("                           ReadBuf start\r\n");
	printf("===================================================================\r\n");
	printf("\r\n");
	return AT24CXX_Print_Buffer(EE_Read_Buffer, EE_SIZE, EE_PAGE_SIZE);
}

/**
 * @fn at24cxx_err_t AT24CXX_Print_Write_Buffer(void)
 * @brief ��ӡAT24CXX�ķ������ݻ���������
 *
 * @return [at24cxx_err_t] ����ִ��״̬
 *			AT24CXX_OK		->		����ִ�гɹ�
 *			AT24CXX_ERROR	->		����ִ��ʧ��
 *
 */
at24cxx_err_t AT24CXX_Print_Write_Buffer(void)
{
	printf("\r\n");
	printf("===================================================================\r\n");
	printf("                          WriteBuf start\r\n");
	printf("===================================================================\r\n");
	printf("\r\n");
	return AT24CXX_Print_Buffer(EE_Write_Buffer, EE_SIZE, EE_PAGE_SIZE);
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
