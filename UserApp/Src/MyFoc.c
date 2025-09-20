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
 * Func: Clark 变换
 * Input : Ialpha, Ibata : alpha/bata相电流
 * Output : Id, Iq : dq轴电流
 */
