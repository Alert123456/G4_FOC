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

float ch[16] = {0};
float t = 0.0f;
uint8_t tail[4] = {0x00, 0x00, 0x80, 0x7f}; // 帧尾标记

void UART_Send()
{

	// 计算正弦波数据
	ch[0] = Motor.theta;
	ch[1] = Motor.Ta;
	ch[2] = Motor.Tb;
	ch[3] = Motor.Tc;
	ch[4] = Motor.Ud;
	ch[5] = Motor.Uq;
	ch[6] = Motor.sector;
	ch[7] = Motor.Ualpha;
	ch[8] = Motor.Ubata;
	ch[9] = Motor.res1;
	ch[10] = Motor.res2;
	ch[11] = Motor.res3;

	// 方法2：直接使用指针转换发送（更简洁）
	HAL_UART_Transmit(&huart2, (uint8_t*)ch, sizeof(float) * 16, HAL_MAX_DELAY);

	// 发送帧尾
	HAL_UART_Transmit(&huart2, tail, 4, HAL_MAX_DELAY);

	HAL_Delay(1);
}
