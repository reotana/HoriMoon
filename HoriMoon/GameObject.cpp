#include "GameObject.h"

GameObject::GameObject() {
	m_nCurrentFrame = 0;				// 프레임 초기화
	m_blsLive = false;					// 모든 기체 생사 여부 초기화
}

GameObject::~GameObject() {

}
//--------------------------------------------------------------------------------------------
//  live
//--------------------------------------------------------------------------------------------
void GameObject::Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval) {
	m_pSprite = pSprite;				// 스프라이트
	m_nCurrentFrame = CurrentFrame;		// 프레임
	m_x = x;							// x 좌표
	m_y = y;							// y 좌표
	m_nFrameInterval = FrameInterval;	// 프레임 간격
	m_blsLive = true;					// 생사 여부(살아있음)
	m_pTimer = timer;					// 타이머
	m_nLastFrameTime = m_pTimer->time();// 프레임이 마지막에 그려진 시간
}

bool GameObject::IsLive() {				// 모든 기체 생사 여부 확인 시 호출
	return m_blsLive;					// 생사 여부 리턴
}

void GameObject::Kill() {					// 죽었을 경우 호출
	m_blsLive = false;					// 기체 쥬금
}

//--------------------------------------------------------------------------------------------
//  Draw
//--------------------------------------------------------------------------------------------
void GameObject::Draw(int x, int y, LPDIRECTDRAWSURFACE7 IpSurface) {
	if (!m_blsLive) 		return;
	if (m_pTimer->elapsed(m_nLastFrameTime, m_nFrameInterval))
		m_nCurrentFrame = ++m_nCurrentFrame % m_pSprite->GetNumberOfFrame();

	m_pSprite->Drawing(m_nCurrentFrame, m_x, m_y, IpSurface);
}

void GameObject::DrawFrame(int x, int y, int nFrame, LPDIRECTDRAWSURFACE7 IpSurface) {
	if (!m_blsLive)		return;

	if (nFrame >= m_pSprite->GetNumberOfFrame())
		nFrame = m_pSprite->GetNumberOfFrame() - 1;
	if (nFrame < 0)
		nFrame = 0;

	m_pSprite->Drawing(nFrame, m_x, m_y, IpSurface);
}

//--------------------------------------------------------------------------------------------
//  Moving 좌표
//--------------------------------------------------------------------------------------------
int GameObject::GetX() {					// x좌표 함수
	return m_x;							// x좌표 리턴
}

int GameObject::GetY() {					// y좌표 함수
	return m_y;							// y좌표 리턴
}