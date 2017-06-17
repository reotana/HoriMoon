//--------------------------------------------------------------------------------------------
//CRandom class 구현
//--------------------------------------------------------------------------------------------

#include<windows.h>

#include"random.h"

CRandom::CRandom() {
	sowseed();
}

CRandom::~CRandom() {

}

void CRandom::sowseed() {
	srand(timeGetTime());
	m_nCount = 0;
}

// m에서 n사이의 난수를 발생 시킨다.
int CRandom::number(int m, int n) {
	if (++m_nCount > 2000)
		sowseed();

	int sample;		// random sample

	if (n<0x7FFF)
		sample = rand();
	else
		sample = ((int)rand() << 15) | (int)rand();	//30bits로 만든다.

	return (sample % (n - m + 1) + m);
}

