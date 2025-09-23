/*
 * MyUart.h
 *
 *  Created on: 2025年9月23日
 *      Author: Alert
 */

#ifndef INC_MYUART_H_
#define INC_MYUART_H_

#include "main.h"

#include "math.h"

// value

 // 使用联合体便于float与byte数组的转换
 typedef union {
     float f_data;
     uint8_t byte[4];
 } float_union_t;




#endif /* INC_MYUART_H_ */
