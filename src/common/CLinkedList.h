#ifndef __CLINKEDLIST_H
#define __CLINKEDLIST_H

#ifndef NULL
#define NULL 0
#endif

template <class T>
class CLinkedList
{
public:

	inline CLinkedList(){ iNext=NULL; }

	inline void SetNext(T *aNext){ iNext=aNext;}
	inline T*& Next(){ return iNext;}

private: 
	T* iNext;
};

#endif


