/*
 * board.h
 *
 *  Created on: Oct 6, 2025
 *      Author: Alert
 */

#ifndef INC_BOARD_H_
#define INC_BOARD_H_

#define ADC_REF_V               (float)(3.3)  // ADC参考电压
#define VBUS_UP_RES             (float)(75.0) // 分压电阻上拉阻值
#define VBUS_DN_RES             (float)(3.0) // 分压电阻
#define VBUS_CONVERSION_FACTOR  (float)(ADC_REF_V*(VBUS_UP_RES+VBUS_DN_RES)/VBUS_DN_RES/4095.0) // 计算母线电压的转换系数

// UVW PWM define start

#define PWM_TIM TIM1
#define PWM_TIM_CLOCK 		120000000

#define PWM_TIM_FREQ 		10000 // HZ
#define PWM_TIM_PULSE 		(PWM_TIM_CLOCK / (2*PWM_TIM_FREQ))
#define PWM_TIM_PULSE_TPWM 	(PWM_TIM_CLOCK / (PWM_TIM_FREQ))

#endif /* INC_BOARD_H_ */
