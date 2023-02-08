#include "sys.h"
#include "usart.h"

#include "string.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2019/9/17
//版本：V1.5
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
//V1.5修改说明
//1,增加了对UCOSII的支持
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB
#if 1

#if (__ARMCC_VERSION >= 6010050)				/* 使用AC6编译器 */
__asm (".global __use_no_semihosting\n\t");		/* 声明不使用半主机模式 */
__asm (".global __ARM_use_no_argv \n\t");		/* AC6不需要声明main函数为无参数格式，否则部分例程可能出现半主机模式 */
#else											/* 使用AC5编译器 */
#pragma import(__use_no_semihosting)

struct __FILE
{
	int handle;
}
#endif

// 不使用半主机模式，至少需要重定义__ttywrch\_sys_exit\_sys_command_string函数，以同时兼容AC6和AC5模式
int _ttywrch(int ch)
{
	ch = ch;
	return ch;
}

// 定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
	x = x;
}

char *_sys_command_string(char *cmd, int len)
{
	return NULL;
}

// FILE在stdio.h里面定义
FILE __stdout;
#endif  /* 代码法 */

#if 1
// 重定义fputc函数
int fputc(int ch, FILE *f)
{
	// 循环发送,直到发送完毕
	while((USART1->ISR&0X40)==0){}
    USART1->TDR = (uint8_t) ch;
	return ch;
}
#endif /* 重定义fputc */

UART_HandleTypeDef huart1;  // UART句柄

/**
 * @fn void USART1_Send_String(uint8_t *str, uint16_t Size)
 * @brief 串口一发送字符串（阻塞式）
 *
 * @param [str] 字符串
 * @param [Size] 字符串长度
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

#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
uint8_t USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收但未读取标志
//bit13~0，	接收到的有效字节数目
uint16_t USART_RX_STA=0;       //接收状态标记	  

uint8_t _1RxBuffer[_1RXBUFFERSIZE];//HAL库使用的串口接收缓冲
  
//初始化IO 串口1 
//bound:波特率
void uart_init(uint32_t bound)
{	
	//UART 初始化设置
	huart1.Instance=USART1;					    	//USART1
	huart1.Init.BaudRate=bound;				    	//波特率
	huart1.Init.WordLength=UART_WORDLENGTH_8B;   	//字长为8位数据格式
	huart1.Init.StopBits=UART_STOPBITS_1;	    	//一个停止位
	huart1.Init.Parity=UART_PARITY_NONE;		    //无奇偶校验位
	huart1.Init.HwFlowCtl=UART_HWCONTROL_NONE;   	//无硬件流控
	huart1.Init.Mode=UART_MODE_TX_RX;		    	//收发模式
	HAL_UART_Init(&huart1);					    	//HAL_UART_Init()会使能UART1
	
	printf("\r\n");
	printf("***************************************************\r\n");
	printf("       STM32G030F6P6 USART1 has initialized!\r\n");
	printf("***************************************************\r\n");
	printf("\r\n");
	
	HAL_UART_Receive_IT(&huart1, (uint8_t *)_1RxBuffer, _1RXBUFFERSIZE);
}
 
//串口1中断服务程序
void USART1_IRQHandler(void)                	
{ 
	uint32_t timeout=0;
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntEnter();    
#endif
	
	HAL_UART_IRQHandler(&huart1);	//调用HAL库中断处理公用函数
     
	timeout=0;
	while(HAL_UART_Receive_IT(&huart1, (uint8_t *)_1RxBuffer, _1RXBUFFERSIZE) != HAL_OK)//一次处理完成之后，重新开启中断并设置RxXferCount为1
	{
		timeout++; //超时处理
		if(timeout>HAL_MAX_DELAY) break;	
	}
	
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntExit();
#endif
} 

/**
 * @fn void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
 * @brief 异步串口接收完成回调函数
 *
 * @param [huart] 异步串口句柄
 *
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)//如果是串口1
	{
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA&0x4000)//接收到了0x0d
			{
				if(_1RxBuffer[0]!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else {  // 接收完成了
					USART_RX_STA|=0x8000;
					USART_RX_BUF[USART_RX_STA&0X3FFF]=0;
				}
			}
			else //还没收到0X0D
			{	
				if(_1RxBuffer[0]==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=_1RxBuffer[0] ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}
	}
}
#endif /* EN_USART1_RX */

//UART底层初始化，时钟使能，引脚配置，中断配置
//此函数会被HAL_UART_Init()调用
//huart:串口句柄

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO端口设置
	GPIO_InitTypeDef GPIO_Initure = {0};
	
	if(huart->Instance==USART1)//如果是串口1，进行串口1 MSP初始化
	{
		__HAL_RCC_USART1_CLK_ENABLE();			//使能USART1时钟
		__HAL_RCC_GPIOB_CLK_ENABLE();			//使能GPIOB时钟
	
		GPIO_Initure.Pin=GPIO_PIN_6|GPIO_PIN_7; //PB6、PB7
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
		GPIO_Initure.Pull=GPIO_NOPULL;			//NO拉
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_LOW; //低速
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//初始化PB6、PB7
		
#if EN_USART1_RX
		HAL_NVIC_SetPriority(USART1_IRQn,3,NULL);		//抢占优先级3
		HAL_NVIC_EnableIRQ(USART1_IRQn);				//使能USART1中断通道
#endif	
	}
}








