#ifndef __CKEYS_H
#define __CKEYS_H

#include "common/defines.h"

enum TPlayerKeys
{
	EKeyLEFT=0,
	EKeyRIGHT,
	EKeyUP, 
	EKeyDOWN, 
	EKeySHOOT, 
	EKeySHIFT, 
	EKeySTRAFE, 
	EKeyLSTRAFE, 
	EKeyRSTRAFE,
	EKeyAmount
};

class CKeys
{
	public:
	int keys[EKeyAmount];
	//void change();
	CKeys& operator =(const CKeys&);
	CKeys();
};

#endif

