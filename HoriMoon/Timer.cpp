//--------------------------------------------------------------------------------------------
//  타이머 쓸 때 사용
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
}												// 결과 값 => 부팅 후 경과 시간(ms)

int CTimer::time() {
	return (timeGetTime() - m_nStartTime);		// 부팅 후 경과 시간 - 게임 시작 시간
}

bool CTimer::elapsed(int &stime, int interval) {
	int current_time = time();
	if (current_time >= stime + interval)		// 마지막으로 해당 장면을 실행한 지(ex- 미사일 쏜지) 딜레이 간격(interval) 만큼 지나면
												//#define MY_MISSILE_INTERVAL 240
	{
		stime = current_time;					// 현재 시간을 미사일을 쏜 시간으로 변경
		return true;							// 미사일을 쏠 수 있게 true
	}
	else
		return false;
}