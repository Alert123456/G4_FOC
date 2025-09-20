/*
 * MyFoc.h
 *
 *  Created on: Sep 20, 2025
 *      Author: 92323
 */

#ifndef INC_MYFOC_H_
#define INC_MYFOC_H_


typedef struct
{
	float ref_pos; // 给定位置
	float ref_spd; // 给定速度

	float Id, Iq;  // dq轴电流
	float Ud, Uq;  // dq轴电压

	float Ialpha, Ibata; // alpha/bata电流
	float Ualpha, Ubata; // alpha/bata电流

	float Ia,Ib,Ic; // 三相电流
	float Ua,Ub,Uc; // 三项电压

}MotorPara;


#endif /* INC_MYFOC_H_ */
