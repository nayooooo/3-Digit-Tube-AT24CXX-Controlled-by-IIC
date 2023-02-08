#include "tim.h"

uint32_t Global_Timer_Tick_Ms = 0;

TIM_HandleTypeDef htim14;

/* TIM14 init function */
//通用定时器14中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器14!
void TIM14_Init(uint16_t arr,uint16_t psc)
{
	htim14.Instance = TIM14;							//通用定时器14
	htim14.Init.Prescaler = psc;						//分频系数
	htim14.Init.CounterMode = TIM_COUNTERMODE_UP;		//向上计数器
	htim14.Init.Period = arr;							//自动装载值
	htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;	//时钟分频因子
	htim14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
	{
		Error_Handler();
	}
	
	HAL_TIM_Base_Start_IT(&htim14); //使能定时器14更新中断：TIM_IT_UPDATE
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{
	if(tim_baseHandle->Instance==TIM14)
	{
		__HAL_RCC_TIM14_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM14_IRQn,3,NULL);	//设置中断优先级，抢占优先级3
		HAL_NVIC_EnableIRQ(TIM14_IRQn);				//开启TIM14中断
	}
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{
	if(tim_baseHandle->Instance==TIM14)
	{
		__HAL_RCC_TIM14_CLK_DISABLE();
	}
}

//定时器14中断服务函数
void TIM14_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntEnter();
#endif
	
    HAL_TIM_IRQHandler(&htim14);
	
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntExit();
#endif
}

//回调函数，定时器中断服务函数调用
#include "main.h"
extern main_Task_Flag_t main_Task_Flag;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim==(&htim14))
    {
        Global_Timer_Tick_Ms++;
		if ((main_Task_Flag.Seg_Count_Task == 0) && (Global_Timer_Tick_Ms != 0)
			&& (Global_Timer_Tick_Ms % Seg_Count_Duty == 0)) {
			main_Task_Flag.Seg_Count_Task = 1;
		}
    }
}
