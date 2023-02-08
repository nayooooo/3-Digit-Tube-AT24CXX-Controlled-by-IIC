#include "sys.h"
#include "usart.h"

#include "string.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2019/9/17
//�汾��V1.5
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB
#if 1

#if (__ARMCC_VERSION >= 6010050)				/* ʹ��AC6������ */
__asm (".global __use_no_semihosting\n\t");		/* ������ʹ�ð�����ģʽ */
__asm (".global __ARM_use_no_argv \n\t");		/* AC6����Ҫ����main����Ϊ�޲�����ʽ�����򲿷����̿��ܳ��ְ�����ģʽ */
#else											/* ʹ��AC5������ */
#pragma import(__use_no_semihosting)

struct __FILE
{
	int handle;
}
#endif

// ��ʹ�ð�����ģʽ��������Ҫ�ض���__ttywrch\_sys_exit\_sys_command_string��������ͬʱ����AC6��AC5ģʽ
int _ttywrch(int ch)
{
	ch = ch;
	return ch;
}

// ����_sys_exit()�Ա���ʹ�ð�����ģʽ
void _sys_exit(int x)
{
	x = x;
}

char *_sys_command_string(char *cmd, int len)
{
	return NULL;
}

// FILE��stdio.h���涨��
FILE __stdout;
#endif  /* ���뷨 */

#if 1
// �ض���fputc����
int fputc(int ch, FILE *f)
{
	// ѭ������,ֱ���������
	while((USART1->ISR&0X40)==0){}
    USART1->TDR = (uint8_t) ch;
	return ch;
}
#endif /* �ض���fputc */

UART_HandleTypeDef huart1;  // UART���

/**
 * @fn void USART1_Send_String(uint8_t *str, uint16_t Size)
 * @brief ����һ�����ַ���������ʽ��
 *
 * @param [str] �ַ���
 * @param [Size] �ַ�������
 * @return [HAL_StatusTypeDef] HAL status
 *			HAL_OK
 *			HAL_ERROR
 *			HAL_BUSY
 *			HAL_TIMEOUT
 *
 */
HAL_StatusTypeDef USART1_Send_String(uint8_t *str, uint16_t Size)
{
	HAL_StatusTypeDef ret;
	
	ret = HAL_UART_Transmit(&huart1, str, Size, 10);
	while (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) != 1) {}
	
	return ret;
}

#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
uint8_t USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	���յ�δ��ȡ��־
//bit13~0��	���յ�����Ч�ֽ���Ŀ
uint16_t USART_RX_STA=0;       //����״̬���	  

uint8_t _1RxBuffer[_1RXBUFFERSIZE];//HAL��ʹ�õĴ��ڽ��ջ���
  
//��ʼ��IO ����1 
//bound:������
void uart_init(uint32_t bound)
{	
	//UART ��ʼ������
	huart1.Instance=USART1;					    	//USART1
	huart1.Init.BaudRate=bound;				    	//������
	huart1.Init.WordLength=UART_WORDLENGTH_8B;   	//�ֳ�Ϊ8λ���ݸ�ʽ
	huart1.Init.StopBits=UART_STOPBITS_1;	    	//һ��ֹͣλ
	huart1.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	huart1.Init.HwFlowCtl=UART_HWCONTROL_NONE;   	//��Ӳ������
	huart1.Init.Mode=UART_MODE_TX_RX;		    	//�շ�ģʽ
	HAL_UART_Init(&huart1);					    	//HAL_UART_Init()��ʹ��UART1
	
	printf("\r\n");
	printf("***************************************************\r\n");
	printf("       STM32G030F6P6 USART1 has initialized!\r\n");
	printf("***************************************************\r\n");
	printf("\r\n");
	
	HAL_UART_Receive_IT(&huart1, (uint8_t *)_1RxBuffer, _1RXBUFFERSIZE);
}
 
//����1�жϷ������
void USART1_IRQHandler(void)                	
{ 
	uint32_t timeout=0;
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntEnter();    
#endif
	
	HAL_UART_IRQHandler(&huart1);	//����HAL���жϴ����ú���
     
	timeout=0;
	while(HAL_UART_Receive_IT(&huart1, (uint8_t *)_1RxBuffer, _1RXBUFFERSIZE) != HAL_OK)//һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
	{
		timeout++; //��ʱ����
		if(timeout>HAL_MAX_DELAY) break;	
	}
	
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntExit();
#endif
} 

/**
 * @fn void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
 * @brief �첽���ڽ�����ɻص�����
 *
 * @param [huart] �첽���ھ��
 *
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)//����Ǵ���1
	{
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(USART_RX_STA&0x4000)//���յ���0x0d
			{
				if(_1RxBuffer[0]!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else {  // ���������
					USART_RX_STA|=0x8000;
					USART_RX_BUF[USART_RX_STA&0X3FFF]=0;
				}
			}
			else //��û�յ�0X0D
			{	
				if(_1RxBuffer[0]==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=_1RxBuffer[0] ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}
	}
}
#endif /* EN_USART1_RX */

//UART�ײ��ʼ����ʱ��ʹ�ܣ��������ã��ж�����
//�˺����ᱻHAL_UART_Init()����
//huart:���ھ��

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO�˿�����
	GPIO_InitTypeDef GPIO_Initure = {0};
	
	if(huart->Instance==USART1)//����Ǵ���1�����д���1 MSP��ʼ��
	{
		__HAL_RCC_USART1_CLK_ENABLE();			//ʹ��USART1ʱ��
		__HAL_RCC_GPIOB_CLK_ENABLE();			//ʹ��GPIOBʱ��
	
		GPIO_Initure.Pin=GPIO_PIN_6|GPIO_PIN_7; //PB6��PB7
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
		GPIO_Initure.Pull=GPIO_NOPULL;			//NO��
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_LOW; //����
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//��ʼ��PB6��PB7
		
#if EN_USART1_RX
		HAL_NVIC_SetPriority(USART1_IRQn,3,NULL);		//��ռ���ȼ�3
		HAL_NVIC_EnableIRQ(USART1_IRQn);				//ʹ��USART1�ж�ͨ��
#endif	
	}
}








