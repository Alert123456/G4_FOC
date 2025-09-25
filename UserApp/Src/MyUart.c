/*
 * MyUart.c
 *
 *  Created on: 2025年9月23日
 *      Author: Alert
 */

#include "MyUart.h"
#include "usart.h"
#include "MyFoc.h"
// value

float ch[8] = {0};
float t = 0.0f;
uint8_t tail[4] = {0x00, 0x00, 0x80, 0x7f}; // 帧尾标记

void UART_Send()
{

	// 计算正弦波数据
	ch[0] = Motor.Ualpha;
	ch[1] = Motor.Ubata;
	ch[2] = Motor.Ua;
	ch[3] = Motor.Ub;
	ch[4] = Motor.Uc;
	ch[5] = Motor.theta;
//	ch[6] = cosf(t);
//	ch[7] = tanf(t);

	// 方法2：直接使用指针转换发送（更简洁）
	HAL_UART_Transmit(&huart2, (uint8_t*)ch, sizeof(float) * 8, HAL_MAX_DELAY);

	// 发送帧尾
	HAL_UART_Transmit(&huart2, tail, 4, HAL_MAX_DELAY);

	HAL_Delay(1);
}
