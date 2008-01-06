#ifndef __ICHARACTEROBSERVER_H
#define __ICHARACTEROBSERVER_H

class CCharacter;

class ICharacterObserver
{
public:
	virtual void CharacterDied(CCharacter* aCharacter)=0;
};

#endif

