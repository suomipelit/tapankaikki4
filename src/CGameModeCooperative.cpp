#include "CGameModeCooperative.h"
#include "CDynGameData.h"
#include "CLevelRuntimeData.h"
#include "IGUIStateController.h"
#include "IGameSubStateController.h"

CGameModeCooperative::CGameModeCooperative( IGUIStateController* aGUIStateController ) : iGUIStateController( aGUIStateController )
{
}

CGameModeCooperative::~CGameModeCooperative()
{
}


bool CGameModeCooperative::ObjectiveFulfilled()
{
	CDynGameData* dyn = iGUIStateController->GameSystem()->DynData();
	if ( dyn->LevelRuntime()->Enemies().empty() )
		return true;
	else
		return false;
}

void CGameModeCooperative::SetupPlayerEpisode( CPlayer* aPlayer )
{
	aPlayer->iShield = 0;
}

void CGameModeCooperative::SetupPlayerLevel( CPlayer* aPlayer )
{
}

bool CGameModeCooperative::GameOver()
{
	CDynGameData* dyn = iGUIStateController->GameSystem()->DynData();
	return dyn->AllPlayersDead();
}

void CGameModeCooperative::CharacterDied( CCharacter* aCharacter )
{	
	CDynGameData* dyn = iGUIStateController->GameSystem()->DynData();
	for (int a=0;a<dyn->LocalPlayers().size();a++)
	{
		if ( static_cast<CCharacter*>(dyn->LocalPlayers()[a])==aCharacter )
		{
			// OK one man down. 
			dyn->RemoveLocalPlayer( dyn->LocalPlayers()[a] );
		}
	}
}

const char* CGameModeCooperative::Name()
{
	return "cooperative";
}

bool CGameModeCooperative::StartFromAnyLevel()
{
	return false;
}

bool CGameModeCooperative::Frags()
{
	return false;
}

bool CGameModeCooperative::DMEpisodes()
{
	return false;
}

bool CGameModeCooperative::CrateRespawn()
{
	return false;
}

bool CGameModeCooperative::ActionTimeMatters()
{
	return true;
}

bool CGameModeCooperative::DisableEnemiesPossible()
{
	return false;
}

bool CGameModeCooperative::ShopEnabled( int aLevel )
{
	if ( aLevel >= 1 )
		return true;
	else
		return false;
}

bool CGameModeCooperative::HidePlayersFromMap()
{
	return false;
}

bool CGameModeCooperative::RandomPlacePlayers()
{
	return false;
}

bool CGameModeCooperative::DMCrates()
{
	return false;
}
