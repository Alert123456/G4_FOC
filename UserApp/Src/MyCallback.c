/*
 * MyCallback.c
 *
 *  Created on: Sep 21, 2025
 *      Author: 92323
 */
// include
#include "main.h"
#include "Myfoc.h"

// value
static uint16_t Timer6_Counter = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM6)   // 判断是哪个定时器 // 10K
    {
    	Foc_Test(&Motor); // 坐标变换验证
    	if(Timer6_Counter >= 100) // 10ms
    	{
    		Theta_Generate(&Motor); // 产生Theta

    		Timer6_Counter = 0;
    	}

        Timer6_Counter++;
    }
}

