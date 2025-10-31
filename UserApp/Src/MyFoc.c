/*
 * MyFoc.c
 *
 *  Created on: Sep 20, 2025
 *      Author: 92323
 */

// Include
# include "MyFoc.h"
#include "math.h"

// Value
MotorPara Motor;

// 定义存放ADC采样值的变量
uint32_t adc_dma_value = 0;

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

void Theta_Generate(MotorPara *m_Motor)
{
	m_Motor->theta += 0.06283f; // 0.06283 = 2*PI/100
	if(m_Motor->theta > 2.0f * M_PI) 
		m_Motor->theta -= 2.0f * M_PI;
}

void Foc_Test(MotorPara *m_Motor)
{
	m_Motor->Ud = 0.0f;
	m_Motor->Uq = 1.0f;
	m_Motor->Tpwm = PWM_TIM_PULSE_TPWM;

	InvPark(m_Motor); // dq -> alpha/bata
//	InvClark(m_Motor);	// alpha/bata -> abc
	SVPWM(m_Motor);
}
void SVPWM(MotorPara *m_Motor)
{
	// 1.扇区判断

	float Vref1,Vref2,Vref3;
	float temp,X,Y,Z,T1,T2,ta,tb,tc;

	 Vref1 = m_Motor->Ubata;
	 Vref2 = 0.5*(sqrtf(3)*m_Motor->Ualpha - m_Motor->Ubata);
	 Vref3 = -0.5*(sqrtf(3)*m_Motor->Ualpha + m_Motor->Ubata);

	if(Vref1 > 0)
		m_Motor->Sector = 1;
	if(Vref2 > 0)
		m_Motor->Sector = m_Motor->Sector + 2;
	if(Vref3 > 0)
		m_Motor->Sector = m_Motor->Sector + 4;
	 temp = sqrtf(3)*m_Motor->Tpwm/m_Motor->Udc;
	 X = temp * m_Motor->Ubata;
	 Y = temp * 0.5 *(sqrtf(3) * m_Motor->Ualpha + m_Motor->Ubata);
	 Z = temp * 0.5 *(-sqrtf(3) * m_Motor->Ualpha + m_Motor->Ubata);

	switch (m_Motor->Sector)
	{
	case 1:
		T1 = Z;
		T2 = Y;
		break;
	case 2:
		T1 = Y;
		T2 = -X;
		break;
	case 3:
		T1 = -Z;
		T2 = X;
		break;
	case 4:
		T1 = -X;
		T2 = Z;
		break;
	case 5:
		T1 = X;
		T2 = -Y;
		break;
	case 6:
		T1 = -Y;
		T2 = -Z;
	default:
		break;
	}

	if(T1 + T2 > m_Motor->Tpwm)
	{
		T1 = T1/(T1 + T2);
		T2 = T2/(T1 + T2);
	}

	ta = (m_Motor->Tpwm - (T1 + T2))/4.0;
	tb = ta + T1/2;
	tc = tb + T2/2;

	switch(m_Motor->Sector)
	{
	case 1:
		m_Motor->Ta = tb;
		m_Motor->Tb = ta;
		m_Motor->Tc = tc;
		break;
	case 2:
		m_Motor->Ta = ta;
		m_Motor->Tb = tc;
		m_Motor->Tc = tb;
		break;
	case 3:
		m_Motor->Ta = tb;
		m_Motor->Tb = ta;
		m_Motor->Tc = tc;
		break;
	case 4:
		m_Motor->Ta = tc;
		m_Motor->Tb = tb;
		m_Motor->Tc = ta;
		break;
	case 5:
		m_Motor->Ta = tc;
		m_Motor->Tb = ta;
		m_Motor->Tc = tb;
		break;
	case 6:
		m_Motor->Ta = tb;
		m_Motor->Tb = tc;
		m_Motor->Tc = ta;
		break;
	default:
		break;
	}

}
// End of file
