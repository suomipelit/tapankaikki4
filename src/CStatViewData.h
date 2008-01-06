#ifndef __CSTATVIEWDATA_H
#define __CSTATVIEWDATA_H

class CStatViewData
{
public:
	CStatViewData();
	void SetDone();
	bool Done();
	int FirstPlayer();
	void SetFirstPlayer(int aPlayer);
	void Reset();

private:
	bool iDone;
	int iPlayer;
};

#endif

