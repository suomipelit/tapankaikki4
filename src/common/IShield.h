#ifndef __ISHIELD_H
#define __ISHIELD_H

class IShield
{
public:
	virtual ~IShield()=0;
	virtual const char* Name() const=0;
	virtual int Cost() const=0;
	virtual int Maximum() const=0;
};

#endif

