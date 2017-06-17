#include "GameObject.h"

GameObject::GameObject() {
	m_nCurrentFrame = 0;				// ������ �ʱ�ȭ
	m_blsLive = false;					// ��� ��ü ���� ���� �ʱ�ȭ
}

GameObject::~GameObject() {

}
//--------------------------------------------------------------------------------------------
//  live
//--------------------------------------------------------------------------------------------
void GameObject::Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval) {
	m_pSprite = pSprite;				// ��������Ʈ
	m_nCurrentFrame = CurrentFrame;		// ������
	m_x = x;							// x ��ǥ
	m_y = y;							// y ��ǥ
	m_nFrameInterval = FrameInterval;	// ������ ����
	m_blsLive = true;					// ���� ����(�������)
	m_pTimer = timer;					// Ÿ�̸�
	m_nLastFrameTime = m_pTimer->time();// �������� �������� �׷��� �ð�
}

bool GameObject::IsLive() {				// ��� ��ü ���� ���� Ȯ�� �� ȣ��
	return m_blsLive;					// ���� ���� ����
}

void GameObject::Kill() {					// �׾��� ��� ȣ��
	m_blsLive = false;					// ��ü ���
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
//  Moving ��ǥ
//--------------------------------------------------------------------------------------------
int GameObject::GetX() {					// x��ǥ �Լ�
	return m_x;							// x��ǥ ����
}

int GameObject::GetY() {					// y��ǥ �Լ�
	return m_y;							// y��ǥ ����
}