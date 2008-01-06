#ifndef __IPLUGINFACTORY_H
#define __IPLUGINFACTORY_H

class IPluginFactory
{
public:
	IPluginFactory(void);
	virtual ~IPluginFactory(void);
	virtual int GameModes();
    virtual IGameMode* GameMode(int a);
	//virtual int Weapons();
 //   virtual IWeaponType* Weapon(int a);
	//virtual int Enemies();
 //   virtual IGameMode* Enemy(int a);
	//virtual int GameModes();
 //   virtual IGameMode* GameMode(int a);
	//virtual int GameModes();
 //   virtual IGameMode* GameMode(int a);
};

#endif
