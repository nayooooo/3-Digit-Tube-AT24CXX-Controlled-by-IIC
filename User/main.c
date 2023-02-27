#include "main.h"

#include "stdlib.h"
#include "string.h"

main_Task_Flag_t main_Task_Flag = {
	.Seg_Count_Task = 0,
};

int main()
{
	uint16_t num = 0;
	
	HAL_Init();
	
	/* System */
	sys_stm32_clock_init();
	delay_Init(MAIN_Fosc_M);
	uart_init(115200UL);
	
	/* BSP */
	Seg_Init();
	AT24CXX_Init();
	
	/* Core */
	TIM14_Init(9, 1599);  // 1000Hz
	
	while(1)
	{
		// 数码管显示数字自加任务
		if (main_Task_Flag.Seg_Count_Task) {
			main_Task_Flag.Seg_Count_Task = 0;
			num++;
			if (num >= 1000) num = 0;
		}
		// 串口接收完成读取并打印AT24CXX数据任务
		if (USART_RX_STA&0x8000) {
			AT24CXX_Read_MultiByte(0, EE_Read_Buffer, EE_SIZE);
			AT24CXX_Print_Read_Buffer();
			USART_RX_STA = 0X0000;
		}
		// 优先级最高的数码管显示任务
		Seg_Show_Num(0, num, 3);
	}
}
