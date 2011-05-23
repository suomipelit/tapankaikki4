#include <stdlib.h>
#include "Types.h"
#include "defines.h"
#include "CColorTable.h"
#include "CGameTypeData.h"

/*******************************************************\
* CBasicShield                                          *
\*******************************************************/
CBasicShield::CBasicShield()
{
}

const char* CBasicShield::Name() const
{
	return "Shield";
}

int CBasicShield::Cost() const
{
	return 160;
}

int CBasicShield::Maximum() const
{
	return 30;
}

/*******************************************************\
* CBodyPartBones                                        *
\*******************************************************/
CBodyPartBones::CBodyPartBones()
{
	iSlow=-0.05f;
	iKickspeed=3.0f;
	iBounce=0;
	iDropBlood=0;
	iSound=ESoundEffectKickHead;
	iSpriteOffset=14*21;
}

/*******************************************************\
* CBodyPartHead                                         *
\*******************************************************/
CBodyPartHead::CBodyPartHead()
{
	iSlow=-0.04f;
	iKickspeed=3.8f;
	iBounce=1;
	iDropBlood=1;
	iSound=ESoundEffectKickHead;
	iSpriteOffset=0*21;
}

/*******************************************************\
* CBodyPartArm                                          *
\*******************************************************/
CBodyPartArm::CBodyPartArm()
{
	iSlow=-0.05f;
	iKickspeed=2.8f;
	iBounce=0;
	iDropBlood=1;
	iSound=ESoundEffectHit;
	iSpriteOffset=1*21;
}

/*******************************************************\
* CBodyPartChest                                        *
\*******************************************************/
CBodyPartChest::CBodyPartChest()
{
	iSlow=-0.07f;
	iKickspeed=2.5f;
	iBounce=0;
	iDropBlood=1;
	iSound=ESoundEffectHit;
	iSpriteOffset=2*21;
}

/*******************************************************\
* CBodyPartEntrails                                     *
\*******************************************************/
CBodyPartEntrails::CBodyPartEntrails()
{	
	iSlow=-0.06f;
	iKickspeed=2.5f;
	iBounce=0;
	iDropBlood=1;
	iSound=ESoundEffectHit;
	iSpriteOffset=3*21;
}

/*******************************************************\
* CBodyPartLeftLeg                                      *
\*******************************************************/
CBodyPartLeftLeg::CBodyPartLeftLeg()
{
	iSlow=-0.05f;
	iKickspeed=2.8f;
	iBounce=0;
	iDropBlood=1;
	iSound=ESoundEffectHit;
	iSpriteOffset=4*21;
}

/*******************************************************\
* CBodyPartRightLeg                                     *
\*******************************************************/
CBodyPartRightLeg::CBodyPartRightLeg()
{
	iSlow=-0.05f;
	iKickspeed=2.8f;
	iBounce=0;
	iDropBlood=1;
	iSound=ESoundEffectHit;
	iSpriteOffset=5*21;
}

/*******************************************************\
* CBodyPartShit                                         *
\*******************************************************/
CBodyPartShit::CBodyPartShit()
{
	iSlow=-0.05f;
	iKickspeed=2.8f;
	iBounce=0;
	iDropBlood=1;
	iSound=ESoundEffectHit;
	iSpriteOffset=6*21;
}

/*******************************************************\
* CBodyPartBurnedChest                                  *
\*******************************************************/
CBodyPartBurnedChest::CBodyPartBurnedChest()
{
	iSlow=-0.05f;
	iKickspeed=3.0f;
	iBounce=0;
	iDropBlood=0;
	iSound=ESoundEffectKickHead;
	iSpriteOffset=12*21;
}

/*******************************************************\
* CBodyPartSkull                                        *
\*******************************************************/
CBodyPartSkull::CBodyPartSkull()
{
	iSlow=-0.04f;
	iKickspeed=4.0f;
	iBounce=1;
	iDropBlood=0;
	iSound=ESoundEffectKickHead;
	iSpriteOffset=13*21;
}

/*******************************************************\
* CEffectBlood                                          *
\*******************************************************/

CEffectBlood::CEffectBlood()
{
	iTCount=4;
	iOrder=1;
	iDelay=5;
	iRandom=0;
	iSprite=1;
	iXS=12;
	iYS=12;
	iAfterDeathEffect = EEffectBloodOnGround;
}

/*******************************************************\
* CEffectSmoke                                         *
\*******************************************************/
CEffectSmoke::CEffectSmoke()
{
	iTCount=4;
	iOrder=0;
	iDelay=5;
	iRandom=0;
	iSprite=1;
	iXS=12;
	iYS=12;
	iOffset=13*4;
	iLightSize=1;
	iLightAddMultiplier=-1;
	iLightTable = ELightTableNormal;
}

/*******************************************************\
* CEffectExplo                                          *
\*******************************************************/
CEffectExplo::CEffectExplo()
{
	iTCount=9;
	iOrder=1;
	iDelay=8;
	iRandom=0;
	iSprite=0;
	iXS=25;
	iYS=25;
	iOffset=320*13;
	iLightSize=7;
	iLightAddConst=5;
	iLightAddMultiplier=-0.8f;
	iLightTable = ELightTableExplosion;
}
/*******************************************************\
* CEffectMetal                                          *
\*******************************************************/
CEffectMetal::CEffectMetal()
{
	iTCount=8;
	iOrder=1;
	iDelay=5;
	iRandom=0;
	iSprite=1;
	iXS=12;
	iYS=12;
	iOffset=13*8;
	iLightAddMultiplier=-1;
	iLightTable = ELightTableNormal;
}

/*******************************************************\
* CEffectMetalYellow                                          *
\*******************************************************/
CEffectMetalYellow::CEffectMetalYellow()
{
	iLightTable = ELightTableYellow;
}

/*******************************************************\
* CEffectMetalRed                                          *
\*******************************************************/
CEffectMetalRed::CEffectMetalRed()
{
	iLightTable = ELightTableRed;
}

/*******************************************************\
* CEffectSmoke2                                          *
\*******************************************************/
CEffectSmoke2::CEffectSmoke2()
{
	iTCount=12;
	iOrder=1;
	iDelay=12;
	iRandom=1;
	iSprite=1;
	iXS=20;
	iYS=20;
	iOffset=320*39;
	iTransparentSprite = true;
}

/*******************************************************\
* CEffectBloodOnGround                                          *
\*******************************************************/
CEffectBloodOnGround::CEffectBloodOnGround()
{
	iTCount=8;
	iOrder=0;
	iDelay=250;
	iRandom=1;
	iSprite=1;
	iXS=6;
	iYS=6;
	iOffset=13*20;
	iLightOLMul=2;
	iHeavyOLMul=3;
	iTransparentSprite = true;
}

/*******************************************************\
* CEffectGunSmoke                                          *
\*******************************************************/
CEffectGunSmoke::CEffectGunSmoke()
{
	iTCount=4;
	iOrder=1;
	iDelay=5;
	iRandom=0;
	iSprite=1;
	iXS=12;
	iYS=12;
	iOffset=13*16;
	iTransparentSprite = true;
}

/*******************************************************\
* CEffectSteam                                          *
\*******************************************************/
CEffectSteam::CEffectSteam()
{
	iTCount=12;
	iOrder=1;
	iDelay=5;
	iRandom=0;
	iSprite=1;
	iXS=20;
	iYS=20;
	iOffset=320*39;
	iTransparentSprite = true;
}

/*******************************************************\
* CEffectTeleport                                       *
\*******************************************************/
CEffectTeleport::CEffectTeleport()
{
	iTCount=7;
	iOrder=1;
	iDelay=5;
	iRandom=0;
	iSprite=1;
	iXS=20;
	iYS=20;
	iOffset=320*60;
	iLightTable = ELightTableNormal;
	iLightSize=4;
}

/*******************************************************\
* CEffectFootprint                                          *
\*******************************************************/

CEffectFootprint::CEffectFootprint()
{
	iTCount=4;
	iOrder=0;
	iDelay=250;
	iRandom=1;
	iSprite=1;
	iXS=6;
	iYS=6;
	iOffset=13*20+320*7;
	iLightOLMul=2;
	iHeavyOLMul=3;
	iTransparentSprite = true;
}

/*******************************************************\
* CEffectFlame                                          *
\*******************************************************/

CEffectFlame::CEffectFlame()
{
	iTCount=1;
	iOrder=1;
	iDelay=12;
	iSprite=0;
	iLightAddMultiplier=-1;
	iLightAddConst=-5;
	iLightTable = ELightTableExplosion;
}

/*******************************************************\
* CEffectFlame2                                          *
\*******************************************************/

CEffectFlame2::CEffectFlame2()
{
	iTCount=1;
	iOrder=1;
	iDelay=38;
	iSprite=0;
	iLightAddMultiplier=-0.33f;
	iLightAddConst=-5;
	iLightTable = ELightTableExplosion;
}

/*******************************************************\
* CEnemyPistolboy                                       *
\*******************************************************/
CEnemyPistolboy::CEnemyPistolboy()
{
	iName="Pistol Boy";
	iSprite=0;
	iSpeed=0.75f;
	iCurrentWeapon=EWeaponPistol;
	iHostile=1;
	iEnergy=18;
	iReward=200;
	iExplosionDeath=0;
}

/*******************************************************\
* CEnemyShotgunManiac                                   *
\*******************************************************/
CEnemyShotgunManiac::CEnemyShotgunManiac()
{
	iName="Shotgun Maniac";
	iSprite=1;
	iSpeed=0.75f;
	iCurrentWeapon=EWeaponShotgun;
	iHostile=1;
	iEnergy=28;
	iReward=350;
	iExplosionDeath=0;
}

/*******************************************************\
* CEnemyUziRebel                                        *
\*******************************************************/
CEnemyUziRebel::CEnemyUziRebel()
{
	iName="Uzi Rebel";
	iSprite=2;
	iSpeed=0.75f;
	iCurrentWeapon=EWeaponUzi;
	iHostile=1;
	iEnergy=40;
	iReward=400;
	iExplosionDeath=0;
}

/*******************************************************\
* CEnemyCommando                                        *
\*******************************************************/
CEnemyCommando::CEnemyCommando()
{
	iName="Commando";
	iSprite=3;
	iSpeed=1.0f;
	iCurrentWeapon=EWeaponAutoRifle;
	iHostile=1;
	iEnergy=50;
	iReward=600;
	iExplosionDeath=0;
}

/*******************************************************\
* CEnemyBazookaBoy                                      *
\*******************************************************/
CEnemyBazookaBoy::CEnemyBazookaBoy()
{
	iName="Bazooka Boy";
	iSprite=4;
	iSpeed=0.75f;
	iCurrentWeapon=EWeaponRocketLauncher;
	iHostile=1;
	iEnergy=40;
	iReward=700;
	iExplosionDeath=1;
}

/*******************************************************\
* CEnemyCivilian                                        *
\*******************************************************/
CEnemyCivilian::CEnemyCivilian()
{
	iName="Civilian";
	iSprite=5;
	iSpeed=0.75f;
	iCurrentWeapon=EWeaponFist;
	iHostile=0;
	iEnergy=18;
	iReward=50;
	iExplosionDeath=0;
}

/*******************************************************\
* CEnemyPunisher                                        *
\*******************************************************/
CEnemyPunisher::CEnemyPunisher()
{
	iName="Punisher";
	iSprite=6;
	iSpeed=0.5f;
	iCurrentWeapon=EWeaponAutoShotgun;
	iHostile=1;
	iEnergy=150;
	iReward=650;
	iExplosionDeath=0;
}

/*******************************************************\
* CEnemyFlamer                                          *
\*******************************************************/
CEnemyFlamer::CEnemyFlamer()
{
	iName="Flamer";
	iSprite=7;
	iSpeed=0.75f;
	iCurrentWeapon=EWeaponFlameThrower;
	iHostile=1;
	iEnergy=100;
	iReward=600;
	iExplosionDeath=1;
}

/*******************************************************\
* CEnemyPunk                                            *
\*******************************************************/
CEnemyPunk::CEnemyPunk()
{
	iName="Punk";
	iSprite=8;
	iSpeed=1.0f;
	iCurrentWeapon=EWeaponFist;
	iHostile=1;
	iEnergy=30;
	iReward=100;
	iExplosionDeath=0;
}

/*******************************************************\
* CEnemyGrenadeGay                                      *
\*******************************************************/
CEnemyGrenadeGay::CEnemyGrenadeGay()
{
	iName="Grenade Gay";
	iSprite=9;
	iSpeed=0.75f;
	iCurrentWeapon=EWeaponGrenadeLauncher;
	iHostile=1;
	iEnergy=70;
	iReward=500;
	iExplosionDeath=1;
}

/*******************************************************\
* CWeaponFist								            *
\*******************************************************/
CWeaponFist::CWeaponFist()
{
	iName="Fist";
	iSprite=0;
	iConsumesBullets=false;
	iCost=0;
	iLoadingTime=25;
	iSmoke=0;
	iBulletAmount=1;
	iAngleAdd=1;
	iGun=false;
	iBulletType=EBulletFist;
	iSound=ESoundEffectHit;
	iPriority=1;
}

/*******************************************************\
* CWeaponPistol                                      *
\*******************************************************/
CWeaponPistol::CWeaponPistol()
{
	iName="Pistol";
	iSprite=1;
	iSingleHand=1;
	iCost=400;
	iLoadingTime=25;
	iSmoke=1;
	iBulletAmount=1;
	iAngleAdd=10;
	iGun=true;
	iBulletType=EBullet9mm;
	iSound=ESoundEffectPistol;
	iPriority=2;
}

/*******************************************************\
* CWeaponShotgun                                      *
\*******************************************************/
CWeaponShotgun::CWeaponShotgun()
{
	iName="Shotgun";
	iSprite=3;
	iCost=1000;
	iLoadingTime=43;
	iSmoke=1;
	iBulletAmount=6;
	iAngleAdd=20;
	iGun=true;
	iBulletType=EBulletShotgunShells;
	iSound=ESoundEffectShotgun;
	iPriority=3;
}

/*******************************************************\
* CWeaponUzi                                      *
\*******************************************************/
CWeaponUzi::CWeaponUzi()
{
	iName="Uzi";
	iSprite=1;
	iSingleHand=1;
	iCost=2000;
	iLoadingTime=10;
	iSmoke=1;
	iBulletAmount=1;
	iAngleAdd=10;
	iGun=true;
	iBulletType=EBullet9mm;
	iSound=ESoundEffectUzi;
	iPriority=4;
}

/*******************************************************\
* CWeaponAutoRifle                                      *
\*******************************************************/
CWeaponAutoRifle::CWeaponAutoRifle()
{
	iName="Auto Rifle";
	iSprite=3;
	iCost=4000;
	iLoadingTime=12;
	iSmoke=1;
	iBulletAmount=1;
	iAngleAdd=10;
	iGun=true;
	iBulletType=EBullet12mm;
	iSound=ESoundEffectAutorifle;
	iPriority=5;
}

/*******************************************************\
* CWeaponRocketLauncher                                 *
\*******************************************************/
CWeaponRocketLauncher::CWeaponRocketLauncher()
{
	iName="Rocket Launcher";
	iSprite=3;
	iCost=4000;
	iLoadingTime=75;
	iSmoke=0;
	iBulletAmount=1;
	iAngleAdd=10;
	iGun=true;
	iBulletType=EBulletMediumRockets;
	iSound=ESoundEffectBazooka;
	iPriority=0;
}

/*******************************************************\
* CWeaponAutoLauncher                                      *
\*******************************************************/
CWeaponAutoLauncher::CWeaponAutoLauncher()
{
	iName="Auto Launcher";
	iSprite=3;
	iCost=6000;
	iLoadingTime=25;
	iSmoke=0;
	iBulletAmount=1;
	iAngleAdd=10;
	iGun=true;
	iBulletType=EBulletLightRockets;
	iSound=ESoundEffectBazooka;
	iPriority=0;
}

/*******************************************************\
* CWeaponHeavyLauncher                                      *
\*******************************************************/
CWeaponHeavyLauncher::CWeaponHeavyLauncher()
{
	iName="Heavy Launcher";
	iSprite=3;
	iCost=6000;
	iLoadingTime=100;
	iSmoke=0;
	iBulletAmount=1;
	iAngleAdd=10;
	iGun=true;
	iBulletType=EBulletHeavyRockets;
	iSound=ESoundEffectBazooka;
	iPriority=0;
}

/*******************************************************\
* CWeaponAutoShotgun                                      *
\*******************************************************/
CWeaponAutoShotgun::CWeaponAutoShotgun()
{
	iName="Auto Shotgun";
	iSprite=3;
	iCost=6000;
	iLoadingTime=12;
	iSmoke=1;
	iBulletAmount=6;
	iAngleAdd=20;
	iGun=true;
	iBulletType=EBulletShotgunShells;
	iSound=ESoundEffectShotgun;
	iPriority=6;
}

/*******************************************************\
* CWeaponC4Activator                                      *
\*******************************************************/
CWeaponC4Activator::CWeaponC4Activator()
{
	iName="C4-Activator";
	iSprite=0;
	iCost=1000;
	iLoadingTime=25;
	iSmoke=0;
	iBulletAmount=1;
	iAngleAdd=1;
	iGun=false;
	iBulletType=EBulletC4Explosives;
	iSound=ESoundEffectNoSound;
	iPriority=0;
}

/*******************************************************\
* CWeaponFlameThrower                                   *
\*******************************************************/
CWeaponFlameThrower::CWeaponFlameThrower()
{
	iName="Flame Thrower";
	iSprite=3;
	iCost=3000;
	iLoadingTime=3;
	iSmoke=0;
	iBulletAmount=1;
	iAngleAdd=10;
	iGun=true;
	iBulletType=EBulletGas;
	iSound=ESoundEffectFlame;
	iPriority=4;
}

/*******************************************************\
* CWeaponMineDropper                                      *
\*******************************************************/
CWeaponMineDropper::CWeaponMineDropper()
{
	iName="Mine Dropper";
	iSprite=0;
	iCost=1000;
	iLoadingTime=50;
	iSmoke=0;
	iBulletAmount=1;
	iAngleAdd=1;
	iGun=false;
	iBulletType=EBulletMines;
	iSound=ESoundEffectNoSound;
	iPriority=0;
}

/*******************************************************\
* CWeaponGrenadeLauncher                                *
\*******************************************************/
CWeaponGrenadeLauncher::CWeaponGrenadeLauncher()
{
	iName="Grenade Launcher";
	iSprite=3;
	iCost=2000;
	iLoadingTime=50;
	iSmoke=1;
	iBulletAmount=1;
	iAngleAdd=10;
	iGun=true;
	iBulletType=EBulletGrenades;
	iSound=ESoundEffectGrenadeLauncher;
	iPriority=0;
}

/*******************************************************\
* CBulletFist                                            *
\*******************************************************/
CBulletFist::CBulletFist()
{
	iName="Fist";
	iType = EBulletTypeFist;
	iStop = 1;
	iSpeed =	5;
	iPower =	3;
	iPushPower =	2;
	iTime = 1;
}
/*******************************************************\
* CBullet9mm                                            *
\*******************************************************/
CBullet9mm::CBullet9mm()
{
	iName="9mm Bullets";
	iType = EBulletTypeNormal;
	iStop = 1;
	iSpeed =	6*18;
	iPower =	5;
	iPushPower =	1;
	iTime = 1;
	iCost = 3;
	iAmountInCrate =	50;
	iMaximumForPlayer	= 300;
}
/*******************************************************\
* CBullet12mm                                            *
\*******************************************************/
CBullet12mm::CBullet12mm()
{
	iName="12mm Bullets";
	iType = EBulletTypeNormal;
	iStop = 1;
	iSpeed =	8*18;
	iPower =	9;
	iPushPower =	1.5;
	iTime = 1;
	iCost = 5;
	iAmountInCrate =	50;
	iMaximumForPlayer	= 300;
}
/*******************************************************\
* CBulletShotgunShells                                            *
\*******************************************************/
CBulletShotgunShells::CBulletShotgunShells()
{		
	iName="Shotgun Shells";
	iType = EBulletTypeNormal;
	iStop = 1;
	iSpeed =	6*18;
	iPower =	3;
	iPushPower =	1.5;
	iTime = 1;
	iCost = 6;
	iAmountInCrate =	20;
	iMaximumForPlayer	= 300;
}

/*******************************************************\
* CBulletRocketBase                                     *
\*******************************************************/
CBulletRocketBase::CBulletRocketBase()
{
	iType = EBulletTypeExplosive;
	iStop = 1;
	iSpeed =	3;
	iTime =(15*18) /	3;
	iLight =	ELightTableNormal;
	iEffect2	= EEffectFlame;
	iEffect = EEffectSmoke2;
	iXS = 7;
	iYS = 7;
}

int CBulletRocketBase::EffectAngle( int aOrigAngle ) const
{
	return aOrigAngle+90+rand()%180;
}

float CBulletRocketBase::EffectSpeed() const
{
	return 0.1f;
}

bool CBulletRocketBase::DoEffect( int aCounter ) const
{
	return (aCounter % 2 == 0);
}

/*******************************************************\
* CBulletLightRockets                                   *
\*******************************************************/
CBulletLightRockets::CBulletLightRockets()
{
	iName="Light Rockets";
	iExplosion =	1;
	iRocketLength = 10;
	iLightSize =	3;
	iCost = 70;
	iAmountInCrate =	15;
	iMaximumForPlayer	= 150;
}
/*******************************************************\
* CBulletMediumRockets                                            *
\*******************************************************/
CBulletMediumRockets::CBulletMediumRockets()
{
	iName="Medium Rockets";
	iExplosion =	2;
	iRocketLength = 13;
	iLightSize =	3;
	iCost = 100;
	iAmountInCrate =	10;
	iMaximumForPlayer	= 125;
}
/*******************************************************\
* CBulletHeavyRockets                                            *
\*******************************************************/
CBulletHeavyRockets::CBulletHeavyRockets()
{
	iName="Heavy Rockets";
	iExplosion =	4;
	iRocketLength = 16;
	iLightSize =	6;
	iCost = 200;
	iAmountInCrate =	5;
	iMaximumForPlayer	= 100;

}
/*******************************************************\
* CBulletC4Explosives                                            *
\*******************************************************/
CBulletC4Explosives::CBulletC4Explosives()
{
	iName="C4-Explosives";
	iType = EBulletTypeExplosive;
	iCollisionTrigger = false;
	iBounceType = KBounceTypeNormal;
	iStop = 1;
	iTime = 250;
	iExplosion =	4;
	iSprite = EGameSpriteBulletC4;
	iXS = 8;
	iYS = 8;
	iCost = 150;
	iAmountInCrate =	5;
	iMaximumForPlayer	= 100;
}
/*******************************************************\
* CBulletGas                                            *
\*******************************************************/
CBulletGas::CBulletGas()
{
	iName="Gas";
	iType = EBulletTypeIncendiary;
	iCollisionTrigger =	false;
	iBounceType = KBounceTypeFollowWall;

	iStop = 1;
	iSpeed =	(int)(3*0.75);
	iPower =	0.2f;
	iTime = 112 / 3;
	iExplosion =	0;
	iLight =ELightTableExplosion;
	iLightSize =	2;
	iLightLuminance =  -	10;
	iLightAdd = 0.4f;
	iCost = 6;
	iAmountInCrate =	50*10;//	div	iMultiplier	= 50
	iMultiplier = 10;
	iSleepTime =	1;
	iMaximumForPlayer	= 300;
}
/*******************************************************\
* CBulletMines                                            *
\*******************************************************/
CBulletMines::CBulletMines()
{
	iName="Mines";
	iType = EBulletTypeExplosive;
	iStop = 1;
	iTime = 6250;
	iExplosion =	1;
	iSprite = EGameSpriteBulletMine;
	iXS = 7;
	iYS = 7;
	iCost = 150;
	iAmountInCrate =	10;
	iSleepTime =	62;
	iMaximumForPlayer	= 100;
}
/*******************************************************\
* CBulletGrenades                                            *
\*******************************************************/
CBulletGrenades::CBulletGrenades()
{
	iName="Grenades";
	iType =	EBulletTypeExplosive;
	iStop =	1;
	iTimeBeforeSpin=38;
	iCollisionTrigger =	false;
	iBounceType = KBounceTypeNormal;
	iBounceSound = ESoundEffectMetalHit;
	iSpeed = 3;
	iSlow =	-0.0225f;
	iTime =	(int)((10*20*1.25f) / 2);
	iExplosion = 2;
	iSprite	= EGameSpriteBulletGrenade;
	iXS	= 7;
	iYS	= 7;
	iSpriteAngles =	40;
	iLightSize = 0;
	iCost =	60;
	iAmountInCrate = 10;
	iMaximumForPlayer = 100;
}
/*******************************************************\
* CBulletExplosiveRay                                            *
\*******************************************************/
CBulletExplosiveRay::CBulletExplosiveRay()
{
	iName="Explosive ray";
	iType =	EBulletTypeNormal;
	iBounceType = KBounceTypeNormal;
	iCollisionTrigger = false;
	iStop =	1;
	iSpeed = 8;
	iPower = 30;
	iPushPower = 5;
	iTime =	0;
	iEffect	= EEffectExplo;
}

int CBulletExplosiveRay::EffectAngle( int aOrigAngle ) const
{
	return aOrigAngle+180;
}

float CBulletExplosiveRay::EffectSpeed() const
{
	return rand()% 2;
}

