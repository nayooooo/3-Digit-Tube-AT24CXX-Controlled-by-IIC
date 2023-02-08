#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
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

/* USART1 ---------------------------------------------------------------------------*/

#define EN_USART1_RX 			1			//ʹ�ܣ�1��/��ֹ��0������1����
#if EN_USART1_RX

#define USART_REC_LEN  			200  		//�����������ֽ��� 200
	  	
extern uint8_t  USART_RX_BUF[USART_REC_LEN]; 	//���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern uint16_t USART_RX_STA;         			//����״̬���	
extern UART_HandleTypeDef huart1; 				//UART���

#define _1RXBUFFERSIZE   1 					//�����С
extern uint8_t _1RxBuffer[_1RXBUFFERSIZE];	//HAL��USART����Buffer

#endif /* EN_USART1_RX */

/* function -------------------------------------------------------------------------*/

HAL_StatusTypeDef USART1_Send_String(uint8_t *str, uint16_t Size);
//����봮���жϽ��գ��벻Ҫע�����º궨��
#if EN_USART1_RX

void uart_init(uint32_t bound);

#endif /* EN_USART1_RX */

#endif /* __USART_H */


