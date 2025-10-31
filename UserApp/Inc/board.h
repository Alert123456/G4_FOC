/*
 * board.h
 *
 *  Created on: Oct 1, 2025
 *      Author: 92323
 */

#ifndef INC_BOARD_H_
#define INC_BOARD_H_

#include "main.h"

// UVW PWM define start
#define PWM_TIM TIM1
#define PWM_TIM_CLOCK 		12000000 // 120M主频

#define PWM_TIM_FREQ 		10000  // 10k
#define PWM_TIM_PULSE 		(PWM_TIM_CLOCK/(2*PWM_TIM_FREQ))
#define PWM_TIM_PULSE_TPWM 	(PWM_TIM_CLOCK/(PWM_TIM_FREQ))

// ADC
#define ADC_REF_V 			(float)(3.3f)
#define VBUS_UP_RES         (float)(75.0f) // 75K
#define VBUS_DOWN_RES       (float)(3.0f) // 3K
#define VBUS_CONVERSION_FACTOR  (ADC_REF_V*((VBUS_UP_RES+VBUS_DOWN_RES)/VBUS_DOWN_RES)/4095.0f) // ADC 12bit

#endif /* INC_BOARD_H_ */
