//--------------------------------------------------------------------------------------------
//  Ÿ�̸� �� �� ���
//--------------------------------------------------------------------------------------------


#include <Windows.h>			
#include <time.h>
#include "timer.h"

CTimer::CTimer() {
	m_nStartTime = 0;
}
CTimer::~CTimer() {

}

void CTimer::start() {
	m_nStartTime = timeGetTime();
}												// ��� �� => ���� �� ��� �ð�(ms)

int CTimer::time() {
	return (timeGetTime() - m_nStartTime);		// ���� �� ��� �ð� - ���� ���� �ð�
}

bool CTimer::elapsed(int &stime, int interval) {
	int current_time = time();
	if (current_time >= stime + interval)		// ���������� �ش� ����� ������ ��(ex- �̻��� ����) ������ ����(interval) ��ŭ ������
												//#define MY_MISSILE_INTERVAL 240
	{
		stime = current_time;					// ���� �ð��� �̻����� �� �ð����� ����
		return true;							// �̻����� �� �� �ְ� true
	}
	else
		return false;
}