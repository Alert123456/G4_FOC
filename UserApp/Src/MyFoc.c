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
	m_Motor->Ibata = (m_Motor->Ia + 2*m_Motor->Ib)/sqrt(3);
}

/*
 * Func: Park 变换
 * Input : Ialpha, Ibata : alpha/bata相电流
 * Output : Id, Iq : dq轴电流
 */
void Park(MotorPara *m_Motor)
{
	m_Motor->Id = m_Motor->Ialpha*cos(m_Motor->theta) + m_Motor->Ibata*sin(m_Motor->theta);
	m_Motor->Iq = -m_Motor->Ialpha*sin(m_Motor->theta) + m_Motor->Ibata*cos(m_Motor->theta);
}

/*
 * Func: 逆Park 变换
 * Input : Ud, Uq : dq轴电压
 * Output : Ualpha, Ubata : alpha/bata相电压
 */
void InvPark(MotorPara *m_Motor)
{
	m_Motor->Ualpha = m_Motor->Ud*cos(m_Motor->theta) - m_Motor->Uq*sin(m_Motor->theta);
	m_Motor->Ubata = m_Motor->Ud*sin(m_Motor->theta) + m_Motor->Uq*cos(m_Motor->theta);
}

/*
 * Func: 逆Clark 变换
 * Input : Ualpha, Ubata : alpha/bata相电压
 * Output : Ua, Ub, Uc : 三相电压
 */
void InvClark(MotorPara *m_Motor)
{
	m_Motor->Ua = m_Motor->Ualpha;
	m_Motor->Ub = -0.5*m_Motor->Ualpha + sqrt(3)*0.5*m_Motor->Ubata;
	m_Motor->Uc = -0.5*m_Motor->Ualpha - sqrt(3)*0.5*m_Motor->Ubata;
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

	InvPark(m_Motor); // dq -> alpha/bata
	InvClark(m_Motor);	// alpha/bata -> abc
}
// End of file