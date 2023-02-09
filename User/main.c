#include "main.h"

#include "string.h"

main_Task_Flag_t main_Task_Flag = {
	.Seg_Count_Task = 0,
};

int main()
{
	uint16_t num = 0;
	
	HAL_Init();
	
	sys_stm32_clock_init();
	delay_Init(MAIN_Fosc_M);
	uart_init(115200UL);
	
	Seg_Init();
	AT24CXX_Init();
	
	TIM14_Init(9, 1599);  // 1000Hz
	
	AT24CXX_Write_OneByte(0, 'a');
	AT24CXX_Write_OneByte(1, 'b');
	AT24CXX_Write_OneByte(2, 'c');
	AT24CXX_Read_MultiByte(0, EE_Read_Buffer, EE_SIZE);
	AT24CXX_Print_Read_Buffer();
	
	while(1)
	{
		if (main_Task_Flag.Seg_Count_Task) {
			main_Task_Flag.Seg_Count_Task = 0;
			num++;
			if (num >= 1000) num = 0;
		}
		if (USART_RX_STA&0x8000) {
			printf("\r\n");
			printf("**************************************\r\n");
			printf("rec: %s\r\n", USART_RX_BUF);
			printf("**************************************\r\n");
			printf("\r\n");
			USART_RX_STA = 0X0000;
		}
		Seg_Show_Num(0, num, 3);
	}
}
