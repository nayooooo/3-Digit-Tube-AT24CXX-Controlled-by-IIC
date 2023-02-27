#ifndef __MAIN_H
#define __MAIN_H

#include "sys.h"
#include "delay.h"
#include "usart.h"

#include "tim.h"
#include "iic.h"

#include "seg.h"
#include "at24cxx.h"

typedef struct{
	uint8_t LED_Flash_Task: 1;
	uint8_t Seg_Count_Task: 1;
}main_Task_Flag_t;

#define LED_Flash_Duty					(1000)
#define Seg_Count_Duty					(1000)

#endif /* __MAIN_H */
