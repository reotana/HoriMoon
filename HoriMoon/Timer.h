#ifndef __timer_h__
#define __timer_h__

class CTimer {
private:
	int m_nStartTime;

public:
	CTimer();
	~CTimer();
	void start();
	int time();

	bool elapsed(int &time, int interval);
};


#endif // !__timer_h__
