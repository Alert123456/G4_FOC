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
uint16_t Timer6_UartCounter = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM6)   // 判断是哪个定时器 // 10K
    {
    	Foc_Test(&Motor); // 坐标变换验证
    	if(Timer6_Counter >= 10) // 1k
    	{
//    		Motor.Udc = adc_dma_value * VBUS_CONVERSION_FACTOR;
    		Theta_Generate(&Motor); // 产生Theta

    		Timer6_Counter = 0;
    	}

        Timer6_Counter++;
        Timer6_UartCounter++;
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if(hadc->Instance == ADC1)
	{
		// adc_dma_value 已经在main.c中定义为全局变量
		// 计算实际电压值

	}
}

