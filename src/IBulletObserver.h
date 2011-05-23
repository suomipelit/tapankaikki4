#ifndef __IBULLETOBSERVER_H_
#define __IBULLETOBSERVER_H_

class CPlayer;
class CEnemy;

class IBulletObserver
{
public:
	virtual void BulletHit( float aShare ) = 0;
	virtual void BulletPlayerKill( CPlayer* aPlayer ) = 0;
	virtual void BulletEnemyKill( CEnemy* aEnemy ) = 0;
};

#endif

