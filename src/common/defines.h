#ifndef __DEFINES_H_
#define __DEFINES_H_

#include "linux.h"
#include "macosx.h"
#include "windows.h"
//#include "vld.h"

enum TNetworkMode
{
	ENetworkModeNone=0,
	ENetworkModeClient,
	ENetworkModeServer
};

#define KNewLine "\n"

const int KCrateWakeupDelay=1250;	// = 12.5 secs if 100 frames per sec
const int KMessageTimeOnScreen=500;	// = 5 secs if 100 frames per sec

const int KLightOverloadLimit=2000;
const int KHeavyOverloadLimit=3000;
const int KMaxEffectsInGame=4000;

const int KMaxPlayers=16;  // max amount of players in game
const int KPlayerColor[]={16*3,16*9,16*10,16*11,16*1,16*2,16*4,16*5};
const int KMaxLocalPlayers=4;

const int KLevelCompleteDelay=940; // level complete delay
const float KEnemyRotationSpeed=3.4f; // computer players rotate speed

const int KTimerFreq=100;

// Sounds...
enum TMenuSoundEffect
{
	EMenuSoundEffectClick,
	EMenuSoundEffectAmount
};

enum TIngameSoundEffect
{
	ESoundEffectNoSound=-1,
	ESoundEffectPistol=0,
	ESoundEffectShotgun,
	ESoundEffectUzi,
	ESoundEffectAutorifle,
	ESoundEffectBazooka,
	ESoundEffectHit,
	ESoundEffectSmallExplosion,
	ESoundEffectExplosion,
	ESoundEffectBigExplosion,
	ESoundEffectClick,
	ESoundEffectDeath,
	ESoundEffectWoosh,
	ESoundEffectFlame,
	ESoundEffectMetalHit,
	ESoundEffectGrenadeLauncher,
	ESoundEffectKickHead,
	ESoundEffectAmount
};

// Network

const int KMaxNetworkDataSize=1024;

enum TMenuSpriteNo
{
	EMenuSpriteEFLogo=0,
	EMenuSpriteMenuBG,
	EMenuSpriteTKLogo,
	EMenuSpriteStatisticsBack,
	EMenuSpriteShopBackNormal,
	EMenuSpriteShopBackHilite,
	EMenuSpriteShopBackHollow,
	EMenuSpriteShopSelection,
	EMenuSpriteVictoryScreen,
	EMenuSpriteAmount
};

enum TGameSprite
{
	EGameSpriteNone=-1,
	EGameSpriteWallClr=0,
	EGameSpriteCrate,
	EGameSpriteShadow,
	EGameSpriteTarget,
	EGameSpriteFloor,
	EGameSpriteFloor2,
	EGameSpriteBodyPart,
	EGameSpriteEffect,
	EGameSpriteWall2,
	EGameSpriteWalls,
	EGameSpriteWalls_1,
	EGameSpriteWalls_2,
	EGameSpriteWalls_3,
	EGameSpriteWalls_4,
	EGameSpriteWalls_5,
	EGameSpriteRambo1,
	EGameSpriteRambo2,
	EGameSpriteRambo3,
	EGameSpriteRambo4,
	EGameSpriteEnemy0,
	EGameSpriteEnemy1,
	EGameSpriteEnemy2,
	EGameSpriteEnemy3,
	EGameSpriteEnemy4,
	EGameSpriteEnemy5,
	EGameSpriteEnemy6,
	EGameSpriteEnemy7,
	EGameSpriteEnemy8,
	EGameSpriteEnemy9,
	EGameSpriteBulletC4,
	EGameSpriteBulletGrenade,
	EGameSpriteBulletMine,
	EGameSpriteAmount
};

enum TLightTable
{
	ELightTableNoLight=-1,
	ELightTableNormal=0,
	ELightTableExplosion,
	ELightTableYellow,
	ELightTableRed,
	ELightTableEnd
};

enum TDirection
{
	ENowhere=-1,
	EUp,
	EDown,
	ELeft,
	ERight
};

enum TBodyPart
{
	EBodyPartHead=0,
	EBodyPartArm,
	EBodyPartChest,
	EBodyPartEntrails,
	EBodyPartLeftLeg,
	EBodyPartRightLeg,
	EBodyPartShit,
	EBodyPartBurnedChest,
	EBodyPartSkull,
	EBodyPartBones,
	EBodyPartAmount
};

// Effects...
enum TEffect
{
	EEffectNoEffect=-1,
	EEffectBlood=0,
	EEffectSmoke,
	EEffectExplo,
	EEffectMetal,
	EEffectMetalYellow,
	EEffectMetalRed,
	EEffectSmoke2,
	EEffectBloodOnGround,
	EEffectGunSmoke,
	EEffectSteam,
	EEffectTeleport,
	EEffectFootprint,
	EEffectFlame,
	EEffectFlame2,
	EEffectAmount
};

enum TEnemy
{
	EEnemyPistolboy=0,
	EEnemyShotgunManiac,
	EEnemyUziRebel,
	EEnemyCommando,
	EEnemyBazookaBoy,
	EEnemyCivilian,
	EEnemyPunisher,
	EEnemyFlamer,
	EEnemyPunk,
	EEnemyGrenadeGay,
	EEnemyAmount
};

enum TWeapon
{
	EWeaponFist=0,
	EWeaponPistol,
	EWeaponShotgun,
	EWeaponUzi,
	EWeaponAutoRifle,
	EWeaponRocketLauncher,
	EWeaponAutoLauncher,
	EWeaponHeavyLauncher,
	EWeaponAutoShotgun,
	EWeaponC4Activator,
	EWeaponFlameThrower,
	EWeaponMineDropper,
	EWeaponGrenadeLauncher,
	EWeaponAmount
};

enum TBullet
{
	EBulletFist=0,
	EBullet9mm,
	EBullet12mm,
	EBulletShotgunShells,
	EBulletLightRockets,
	EBulletMediumRockets,
	EBulletHeavyRockets,
	EBulletC4Explosives,
	EBulletGas,
	EBulletMines,
	EBulletGrenades,
	EBulletExplosionRay,
	EBulletAmount
};


#endif

