#include "CGameModeDeathmatch.h"
#include "CDynGameData.h"
#include "IGUIStateController.h"
#include "IGameSubStateController.h"
#include "CLevelRuntimeData.h"

CGameModeDeathmatch::CGameModeDeathmatch( IGUIStateController* aGUIStateController ) : iGUIStateController( aGUIStateController )
{
}

CGameModeDeathmatch::~CGameModeDeathmatch()
{
}

bool CGameModeDeathmatch::ObjectiveFulfilled()
{
	return false;
}

void CGameModeDeathmatch::CharacterDied( CCharacter* aCharacter )
{
	CDynGameData* dyn = iGUIStateController->GameSystem()->DynData();
	for (int a=0;a<dyn->LocalPlayers().size();a++)
	{
		if ( static_cast<CCharacter*>(dyn->LocalPlayers()[a])==aCharacter )
		{
			COptions* opt = iGUIStateController->Options();
			CPlayer* ply = dyn->LocalPlayers()[a];

			ASSERT( ply->iDead );
			// teleport effect at place of death
			dyn->LevelRuntime()->NewEffect(EEffectTeleport, ply->Location().Add(14,14), 0, 0);

			ply->TotalReset( dyn->PlayerDrawRect(a) );
			ply->Reset( dyn->PlayerDrawRect(a) );
			ply->SetSpeed( opt->Data().iDMSpeed );

			ply->RandomPlace();

			ply->InitScr( dyn->PlayerDrawRect(a) );

			dyn->LevelRuntime()->NewEffect(EEffectTeleport, ply->Location().Add(14,14), 0, 0);
			
			// DM Energy
			ply->iShield = 30;
			ply->iEnergy = (float)(KPlayerMaxEnergy+ply->iShield*KShieldEnergyAmount)/3;

			if ( opt->Data().iDMTargetSystem )
				ply->iTargetSystem = true;
		}
	}
}

void CGameModeDeathmatch::SetupPlayerEpisode( CPlayer* aPlayer )
{
	aPlayer->iShield = 30;
}

void CGameModeDeathmatch::SetupPlayerLevel( CPlayer* aPlayer )
{
	COptions* opt = iGUIStateController->Options();
	aPlayer->iEnergy = (KPlayerMaxEnergy+(aPlayer->iShield*KShieldEnergyAmount))/3;
	if ( opt->Data().iDMTargetSystem )
	{
		aPlayer->iTargetSystem = true;
	}
	aPlayer->SetSpeed( opt->Data().iDMSpeed );
}

const char* CGameModeDeathmatch::Name()
{
	return "deathmatch";
}

bool CGameModeDeathmatch::StartFromAnyLevel()
{
	return true;
}

bool CGameModeDeathmatch::GameOver()
{
	return false;
}

bool CGameModeDeathmatch::Frags()
{
	return true;
}

bool CGameModeDeathmatch::DMEpisodes()
{
	return true;
}

bool CGameModeDeathmatch::CrateRespawn()
{
	return true;
}

bool CGameModeDeathmatch::ActionTimeMatters()
{
	return false;
}

bool CGameModeDeathmatch::DisableEnemiesPossible()
{
	return true;
}

bool CGameModeDeathmatch::ShopEnabled( int aLevel )
{
	return false;
}

bool CGameModeDeathmatch::HidePlayersFromMap()
{
	return true;
}

bool CGameModeDeathmatch::RandomPlacePlayers()
{
	return true;
}

bool CGameModeDeathmatch::DMCrates()
{
	return true;
}
