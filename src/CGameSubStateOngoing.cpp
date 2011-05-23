#include "common/CEventHandler.h"

#include "CGameSubStateOngoing.h"
#include "IGameSubStateController.h"
#include "IGUIStateController.h"
#include "CGameState.h"
#include "CDynGameData.h"
#include "CGameDrawManager.h"
#include "CGameGraphicsInterface.h"
#include "timer.h"
#include "CSoundPlayer.h"
#include "CIngameData.h"
#include "CGameStatisticsData.h"
#include "CLevelRuntimeData.h"

namespace
{
	const int KMaxMouseRotationPerFrame=15; // max players mouse rotation per frame -unski mode-
};

CGameSubStateOngoing::CGameSubStateOngoing(IGameSubStateController* aSubStateController): iStateController (aSubStateController)
{
	iDynData = aSubStateController->DynData();
	iEH  = aSubStateController->GUIStateController()->EventHandler();
}

CGameSubStateOngoing::~CGameSubStateOngoing()
{
}

// return true if draw is needed
bool CGameSubStateOngoing::RunFrame()
{
	if ( iFadeIn )
	{
		iStateController->GUIStateController()->StartFadeIn( iDynData->LevelRuntime()->IngameData()->Palette() );
		iFadeIn = false;
	}

	iClockval=TimerCounter;

	if (iClockval>iDynData->LastFrame())
	{
		int frameCount = iDynData->LowFramerate();

		for (;frameCount;frameCount--)
		{
			CheckUserInput();

			iDynData->LevelRuntime()->PerFrameCalc();

			iDynData->PushPlayers();
			iDynData->CheckBurn();

			if (iDynData->GameMode()->CrateRespawn() &&
				iDynData->NetworkMode()!=ENetworkModeClient)
				iDynData->LevelRuntime()->ChkWakeUps();

			for(int a=0; a<iDynData->LocalPlayers().size(); a++)
			{ 
				iDynData->LocalPlayers()[a]->MoveScr(iDynData->PlayerDrawRect(a));
				iDynData->LocalPlayers()[a]->LoadWeapons();
			}

			iDynData->AnimatePlayers();

			if (iDynData->NetworkMode()!=ENetworkModeClient)
			{
				iDynData->LevelRuntime()->EnemyUpdate();
			}

			iDynData->DecHits();
			iDynData->ShowSteam();
			
			if (iDynData->iNextLevel) 
				iDynData->iNextLevel++;

			iDynData->LevelRuntime()->RollEffects();
		}	

		// Check if enemies are dead
		if (iDynData->GameMode()->ObjectiveFulfilled())
			if (iDynData->iNextLevel<=0 )
				iDynData->iNextLevel=1;

		/* Are we dead? */
		if ( iDynData->AllPlayersDead() )
		{
			iDynData->iCompleteTime = iDynData->LevelRuntime()->Level().iGeneralLevelInfo.iTimeLimit;
			iDynData->Statistics()->SetLatestLevel( iDynData->CurrentLevel() );
			iDynData->NextLevel();
			iStateController->GUIStateController()->StartZoomOut(CState(CState::EMainStateGame,EGameStateStatistics));
		}

		/* Next level?  */
		if (iDynData->iNextLevel>=KLevelCompleteDelay || iDynData->iUserQuit)
		{
			iDynData->Statistics()->SetLatestLevel( iDynData->CurrentLevel() );
			iDynData->NextLevel();
			iStateController->GUIStateController()->StartFadeOut(CState(CState::EMainStateGame,EGameStateStatistics));
		}

		return true;
	}
	return false;
}

void CGameSubStateOngoing::Draw(CDrawArea& aDirtyArea,CDrawArea& aDrawArea,CGameGraphicsInterface* aGGI)
{
	iStateController->GameDrawManager()->DrawFrame(aDrawArea,iEH);

	iDynData->FPSCounter( iClockval );
}

void CGameSubStateOngoing::EnterState()
{
	std::string tmp,tmp2;
	iFadeIn = true;
	CEpisode* epi = iStateController->GUIStateController()->GameData()->Options()->EpisodeList()->Episode( iDynData->Episode(), iDynData->GameMode()->DMEpisodes() );
	const char* levname = epi->Level( iDynData->CurrentLevel() );
	CIngameData* data = iStateController->DynData()->LevelRuntime()->IngameData();

	tmp = epi->DirName();
	tmp+= "/default.ini";

	tmp2 = levname;
	tmp2 = tmp2.substr(0,tmp2.length()-4) + ".ini";// ASSERT that files always have .lev extension

	data->LoadIngameData( tmp, tmp2 );
	iStateController->GUIStateController()->SoundPlayer()->GameMusic( data->Soundtrack().c_str(), data->SongNumber() );
	iStateController->GUIStateController()->SoundPlayer()->SetObserver(this);
	iStateController->GUIStateController()->GGI()->SetGameMode();

	// Last resort for player setup before actual game start
	iDynData->PreparePlayersForLevel();

	// Just clear stack from the keypresses
	iEH->ResetStack();

	// Get mouse in non-fullscreen modes
	iEH->GrabInputs(true);

	/* These must be initialized here */
	iDynData->ResetClocks();
	iClockval=iDynData->StartTime();
	iDynData->iNextLevel=0;

	CMessageBoard* mb=iStateController->GUIStateController()->MessageBoard();
	// move messageboard a little bit if deathmatch
	if ( iDynData->GameMode()->Frags() )
		mb->SetDrawOffset( 16 );
	else
		mb->SetDrawOffset( 0 ); // just make sure

	std::vector< CEnemy >::iterator iter = iDynData->LevelRuntime()->Enemies().begin();
	while(iter != iDynData->LevelRuntime()->Enemies().end() )
	{
		CCoord<int> pos = iter->Location().Add(KBlockSpriteSize/2,KBlockSpriteSize/2).Div(KBlockSpriteSize);
		
		if (!iDynData->LevelRuntime()->Level().Reachable(pos))
		{
			mb->AddMessage("One or more enemies are placed in invalid positions!");
			mb->AddMessage("Those enemies cannot be killed and this level cannot be solved.");
			mb->AddMessage("Please fix those issues with level editor!");
			mb->AddMessage("(This should however never occure)");
			break;
		}
		++iter;
	}
	
}

void CGameSubStateOngoing::ExitState()
{
	iStateController->DynData()->LevelRuntime()->IngameData()->UnloadIngameData();
	iStateController->GUIStateController()->SoundPlayer()->SetObserver(NULL);
	iStateController->GUIStateController()->GGI()->SetMenuMode();
	iStateController->GUIStateController()->SoundPlayer()->StopMusic();

	iStateController->GUIStateController()->MessageBoard()->SetDrawOffset( 0 );
}

void CGameSubStateOngoing::SoundHeard(enum TIngameSoundEffect aEffect,float aVolume,CCoord<int> aPos)
{
	std::vector< CEnemy >::iterator iter = iDynData->LevelRuntime()->Enemies().begin();

	while(iter != iDynData->LevelRuntime()->Enemies().end() )
	{
		iter->SoundHeard(aEffect,aVolume,aPos);
		++iter;
	};
}

void CGameSubStateOngoing::CheckUserInput() 
{
	char text[100];
	float temp;
    int a;
	int key;

	CEventHandler* eh = iStateController->EventHandler();
	
	if (!iDynData->MessageWritingMode()&&eh->Kbhit())
	{
		key=eh->Getch().sym;

		if (key==SDLK_RETURN && iDynData->NetworkMode()!=ENetworkModeNone) 
		{
			eh->State(SDLK_RETURN)=0;
			iDynData->SetMessageWritingMode(true);
			iDynData->NetMsgReset();
		}
		else
			if (key==SDLK_ESCAPE)
			{
				eh->State(SDLK_ESCAPE)=0;
				eh->GrabInputs(false);
				iStateController->GUIStateController()->State()->SetStateAndSave(CState(CState::EMainStateGame,EGameStateQuit));
				saveTimer();
				eh->ResetStack();
			}
	}

	if (iDynData->MessageWritingMode())
	{
		WriteNetworkMessage( eh );
	}
	
	if (eh->State(SDLK_PAGEUP)) 
	{
        if (iStateController->GUIStateController()->Options()->Data().iMouseSensitivity<KMouseSensitivityMax) 
			iStateController->GUIStateController()->Options()->Data().iMouseSensitivity+=KMouseSensitivityStep; 
	        ASSERT(_snprintf(text,100,"Mouse sensitivity %.2f",iStateController->GUIStateController()->Options()->Data().iMouseSensitivity*100)>0);
		iStateController->GUIStateController()->MessageBoard()->AddMessage(text);
		eh->State(SDLK_PAGEUP)=0;
    }

	if (eh->State(SDLK_PAGEDOWN)) 
	{
        if (iStateController->GUIStateController()->Options()->Data().iMouseSensitivity>KMouseSensitivityMin) 
			iStateController->GUIStateController()->Options()->Data().iMouseSensitivity-=KMouseSensitivityStep; 

	        ASSERT(_snprintf(text,100,"Mouse sensitivity %.2f",iStateController->GUIStateController()->Options()->Data().iMouseSensitivity*100)>0);
			iStateController->GUIStateController()->MessageBoard()->AddMessage(text);
        eh->State(SDLK_PAGEDOWN)=0;
    }

    if (eh->State(SDLK_F4))
	{
		++iStateController->GUIStateController()->Options()->Data().iMouseMode%=iDynData->LocalPlayers().size()+1;
		iDynData->UpdateMouseOwner();
		ASSERT(_snprintf(text,100,"Mouse mode: %s",iStateController->GUIStateController()->Options()->MouseModeText())>0);
		iStateController->GUIStateController()->MessageBoard()->AddMessage(text);
        eh->State(SDLK_F4)=0;
    }
	
	// Hide/Show Map
	if (eh->State(SDLK_F5)) 
	{
		iDynData->iShowMap=!iDynData->iShowMap;
		eh->State(SDLK_F5)=0;
	}

	// Hide/Show framerate
	if (eh->State(SDLK_F6)) 
	{
		iDynData->iDrawFrames=!iDynData->iDrawFrames;
		eh->State(SDLK_F6)=0;
	}

	// Player key handling
	for(a=0; a<iDynData->LocalPlayers().size(); a++) 
		if (!iDynData->LocalPlayers()[a]->iDead) 
			iDynData->LocalPlayers()[a]->CheckKeys(a,eh, 
			iDynData->PlayerDrawRect(a));

	/* Handle mouse input */
	if (iDynData->MouseOwner()&&
		!iDynData->MouseOwner()->iDead)
		{
			temp=eh->GetMouse().XRel()*((float)iStateController->GUIStateController()->Options()->Data().iMouseSensitivity);

			if (temp<-KMaxMouseRotationPerFrame) temp=-KMaxMouseRotationPerFrame;
			if (temp>KMaxMouseRotationPerFrame)  temp=KMaxMouseRotationPerFrame;

			iDynData->MouseOwner()->Rotate(temp);
			eh->GetMouse().SetXRel(0);
			eh->GetMouse().SetYRel(0);

				// Check mouse wheel down
			if (eh->GetMouse().Button(CMouse::EButtonWheelDown))
			{
				iDynData->MouseOwner()->ChangeWeapon(false);
				eh->GetMouse().DecButton(CMouse::EButtonWheelDown);
			}

			// Check mouse wheel up
			if (eh->GetMouse().Button(CMouse::EButtonWheelUp))
			{
				iDynData->MouseOwner()->ChangeWeapon(true);
				eh->GetMouse().DecButton(CMouse::EButtonWheelUp);
			}
		}
}

void CGameSubStateOngoing::WriteNetworkMessage( CEventHandler* aEventHandler )
{
	int a;
	char tmp[KMaxMessagesLengthOnBoard];

	if (!aEventHandler->Kbhit()) return;

	a=aEventHandler->Getch().sym;

	switch (a)
	{
	case SDLK_BACKSPACE:
		iDynData->NetMsgEraseChar();
		break;

	case SDLK_RETURN:
		iDynData->SetMessageWritingMode(false);
		strcpy(tmp,iDynData->LocalPlayers()[0]->iName);
		strcat(tmp,": ");
		strcat(tmp,iDynData->NetMsg());

		iStateController->GUIStateController()->MessageBoard()->AddMessage(tmp);
		break;

	case SDLK_ESCAPE:
		iDynData->SetMessageWritingMode(false);
		break;

	default:
		iDynData->NetMsgAppendChar((char)a);

	}
}
