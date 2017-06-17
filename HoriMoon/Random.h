// 난수를 발생

#ifndef __random_h__
#define __random_h__

class CRandom {
private:
	int m_nCount;
	void sowseed();
public:
	CRandom();
	~CRandom();
	int number(int m, int n);
};

#endif // !__random_h__
