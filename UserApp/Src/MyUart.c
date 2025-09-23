/*
 * MyUart.c
 *
 *  Created on: 2025年9月23日
 *      Author: Alert
 */

#include "MyUart.h"
#include "usart.h"
// value

float ch[4] = {0};
float t = 0.0f;
uint8_t tail[4] = {0x00, 0x00, 0x80, 0x7f}; // 帧尾标记

void UART_Send()
{
	// 协议发送
	if(t > 2 * 3.14)
	  t = 0.0f;
	t += 0.1f;

	// 计算正弦波数据
	ch[0] = sinf(t);
	ch[1] = cosf(t);
	ch[2] = tanf(t);
	//	  ch[3] = sqrt(t);

	// 方法1：使用联合体发送（推荐，可读性更好）
	//	  float_union_t temp;
	//	  for(int i = 0; i < 4; i++) {
	//		  temp.f_data = ch[i];
	//		  HAL_UART_Transmit(&huart8, temp.byte, 4, HAL_MAX_DELAY);
	//	  }

	// 方法2：直接使用指针转换发送（更简洁）
	HAL_UART_Transmit(&huart2, (uint8_t*)ch, sizeof(float) * 4, HAL_MAX_DELAY);

	// 发送帧尾
	HAL_UART_Transmit(&huart2, tail, 4, HAL_MAX_DELAY);

	HAL_Delay(1);
}
