/*
 * MyFoc.c
 *
 *  Created on: Sep 20, 2025
 *      Author: 92323
 */

// Include
#include "MyFoc.h"
#include "math.h"
#include "adc.h"
#include "board.h"

// Value
MotorPara Motor;
uint32_t Udc_adc = 0;
// Func

void MotorInit(MotorPara *m_Motor)
{
	m_Motor->Ua = 0;
	m_Motor->Ub = 0;
	m_Motor->Uc = 0;
}

/*
 * Func: Clark 变换
 * Input : Ia, Ib, Ic : 三相电流
 * Output : Ialpha, Ibata : alpha/bata相电流
 */
void Clark(MotorPara *m_Motor)
{
	m_Motor->Ic = -(m_Motor->Ia + m_Motor->Ib);
	m_Motor->Ialpha = m_Motor->Ia;
	m_Motor->Ibata = (m_Motor->Ia + 2*m_Motor->Ib)/sqrtf(3);
}

/*
 * Func: Park 变换
 * Input : Ialpha, Ibata : alpha/bata相电流
 * Output : Id, Iq : dq轴电流
 */
void Park(MotorPara *m_Motor)
{
	m_Motor->Id = m_Motor->Ialpha*cosf(m_Motor->theta) + m_Motor->Ibata*sinf(m_Motor->theta);
	m_Motor->Iq = -m_Motor->Ialpha*sinf(m_Motor->theta) + m_Motor->Ibata*cosf(m_Motor->theta);
}

/*
 * Func: 逆Park 变换
 * Input : Ud, Uq : dq轴电压
 * Output : Ualpha, Ubata : alpha/bata相电压
 */
void InvPark(MotorPara *m_Motor)
{
	m_Motor->Ualpha = m_Motor->Ud*cosf(m_Motor->theta) - m_Motor->Uq*sinf(m_Motor->theta);
	m_Motor->Ubata = m_Motor->Ud*sinf(m_Motor->theta) + m_Motor->Uq*cosf(m_Motor->theta);
}

/*
 * Func: 逆Clark 变换
 * Input : Ualpha, Ubata : alpha/bata相电压
 * Output : Ua, Ub, Uc : 三相电压
 */
void InvClark(MotorPara *m_Motor)
{
	m_Motor->Ua = m_Motor->Ualpha;
	m_Motor->Ub = -0.5*m_Motor->Ualpha + sqrtf(3)*0.5*m_Motor->Ubata;
	m_Motor->Uc = -0.5*m_Motor->Ualpha - sqrtf(3)*0.5*m_Motor->Ubata;
}
/*
 * Func: 产生三角波
 * Input :
 * Output :
 */
void Theta_Generate(MotorPara *m_Motor)
{
	m_Motor->theta += 0.06283f; // 0.06283 = 2*PI/100
	if(m_Motor->theta > 2.0f * M_PI) 
		m_Motor->theta -= 2.0f * M_PI;
}

/*
 * Func: 母线电压读取
 * Input : Udc_adc ：ADC采样值
 * Output : 母线电压Udc
 */
void Vbus_Sense(MotorPara *m_Motor)
{
	// 启动ADC转换
	HAL_ADC_Start(&hadc1);
	// 等待转换完成，超时时间设为10ms
	if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
	{
		// 读取ADC转换值
		Udc_adc = HAL_ADC_GetValue(&hadc1);
		// 将ADC值转换为电压值 (假设参考电压为3.3V)
		m_Motor->Udc = (float)Udc_adc * VBUS_CONVERSION_FACTOR;
		// 此处可以通过串口打印电压值，或进行其他处理
		// printf("ADC Value: %d, Voltage: %.2f V\r\n", adc_value, voltage);
	}
	HAL_ADC_Stop(&hadc1);
}
/*
 * Func: SVPWM与扇区设置
 * Input : Ualpha, Ubata : alpha/bata相电压
 * Output : 扇区sector，占空比Ta, Tb, Tc
 */
void SVPWM(MotorPara *m_Motor)
{
	// Value
	float A, B, C;
	float X, Y, Z;
	float Tx, Ty,Txy;
	float ta, tb,tc;
	float Tcmp1, Tcmp2,Tcmp3;
	uint8_t sector = 0;

	float Ualpha = m_Motor->Ualpha;
	float Ubata = m_Motor->Ubata;
	// 计算扇区
	A = Ubata;
	B = (sqrtf(3)*Ualpha-Ubata)/2;
	C = (-sqrtf(3)*Ualpha-Ubata)/2;
	if(A > 0)
		sector = 1;
	if(B > 0)
		sector += 2;
	if(C > 0)
		sector += 4;
	X = sqrtf(3)*m_Motor->Tpwm/m_Motor->Udc*Ubata;
	Y = sqrtf(3)*m_Motor->Tpwm/m_Motor->Udc*(0.5*(sqrtf(3)*Ualpha+Ubata));
	Z = sqrtf(3)*m_Motor->Tpwm/m_Motor->Udc*(0.5*(-sqrtf(3)*Ualpha+Ubata));
	switch (sector)
	{
	case 1:
		Tx = Z;
		Ty = Y;
		break;
	case 2:
		Tx = Y;
		Ty = -X;
		break;
	case 3:
		Tx = -Z;
		Ty = X;
		break;
	case 4:
		Tx = -X;
		Ty = Z;
		break;
	case 5:
		Tx = X;
		Ty = -Y;
		break;
	case 6:
		Tx = -Y;
		Ty = -Z;
		break;
	default:
		break;
	}

	if(Tx + Ty > m_Motor->Tpwm)
	{
		Tx = Tx/(Tx+Ty);
		Ty = Ty/(Tx+Ty);
	}
	else
	{
		Tx = Tx;
		Ty = Ty;
	}


	ta = (float)(m_Motor->Tpwm-Tx-Ty)/4.0;
	tb = ta+Tx/2;
	tc = tb+Ty/2;
	switch (sector)
	{
	case 1:
		Tcmp1 = tb;
		Tcmp2 = ta;
		Tcmp3 = tc;
		break;
	case 2:
		Tcmp1=ta;
		Tcmp2=tc;
		Tcmp3=tb;
		break;
	case 3:
		Tcmp1=ta;
		Tcmp2=tb;
		Tcmp3=tc;
		break;
	case 4:
		Tcmp1=tc;
		Tcmp2=tb;
		Tcmp3=ta;
		break;
	case 5:
		Tcmp1=tc;
		Tcmp2=ta;
		Tcmp3=tb;
		break;
	case 6:
		Tcmp1=tb;
		Tcmp2=tc;
		Tcmp3=ta;
	default:
		break;
	}
	m_Motor->Ta = Tcmp1;
	m_Motor->Tb = Tcmp2;
	m_Motor->Tc = Tcmp3;
	m_Motor->sector = sector;

}

 // 固定在最后
void Foc_Test(MotorPara *m_Motor)
{
	m_Motor->Ud = 0.0f;
	m_Motor->Uq = 1.0f;
	m_Motor->Tpwm = PWM_TIM_PULSE_TPWM;
	InvPark(m_Motor); // dq -> alpha/bata
//	InvClark(m_Motor);	// alpha/bata -> abc
	SVPWM(m_Motor);
}
// End of file
