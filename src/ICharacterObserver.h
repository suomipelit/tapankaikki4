#ifndef __ICHARACTEROBSERVER
#define __ICHARACTEROBSERVER

class CCharacter;

class ICharacterObserver
{
public:
	virtual void CharacterDied(CCharacter* aCharacter)=0;
};

#endif

