#include "CEventFilter.h"

#include "CMessageBoard.h"
#include "CGameGraphicsInterface.h"
#include "IGUIStateController.h"
#include "common/CEventHandler.h"

CEventFilter::CEventFilter(IGUIStateController* aGA): iGA(aGA)
{
}

CEventFilter::~CEventFilter(void)
{
}

bool CEventFilter::HandleEvent(SDL_Event event)
{
	char text[256];

	switch( event.type )
		{	
			case SDL_ACTIVEEVENT:
				iGA->SetActive( event.active.gain!=0 );
				return true;

			case SDL_VIDEOEXPOSE:
				iGA->SetFullScreenUpdate( true );
				return true;

            case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
					case SDLK_HOME:

						iGA->Options()->Data().iEffectsVolume=min(iGA->Options()->Data().iEffectsVolume+KEffectVolumeStep,KEffectVolumeMax);
						iGA->Options()->UpdateAudioChanges();
						ASSERT(_snprintf(text,256,"Effect volume: %.0f%%",100*iGA->Options()->Data().iEffectsVolume)>0);
						iGA->MessageBoard()->AddMessage(text);
						return true;

					case SDLK_END:
				
						iGA->Options()->Data().iEffectsVolume=max(iGA->Options()->Data().iEffectsVolume-KEffectVolumeStep,KEffectVolumeMin);
						iGA->Options()->UpdateAudioChanges();
						ASSERT(_snprintf(text,256,"Effect volume: %.0f%%",100*iGA->Options()->Data().iEffectsVolume)>0);
						iGA->MessageBoard()->AddMessage(text);
						return true;

					case SDLK_INSERT:
				
						iGA->Options()->Data().iMusicVolume=min(iGA->Options()->Data().iMusicVolume+KMusicVolumeStep,KMusicVolumeMax);
						iGA->Options()->UpdateAudioChanges();
						ASSERT(_snprintf(text,256,"Music volume: %.0f%%",100*iGA->Options()->Data().iMusicVolume)>0);
						iGA->MessageBoard()->AddMessage(text);
						return true;

					case SDLK_DELETE:
			
						iGA->Options()->Data().iMusicVolume=max(iGA->Options()->Data().iMusicVolume-KMusicVolumeStep,KMusicVolumeMin);
						iGA->Options()->UpdateAudioChanges();
						ASSERT(_snprintf(text,256,"Music volume: %.0f%%",100*iGA->Options()->Data().iMusicVolume)>0);
						iGA->MessageBoard()->AddMessage(text);
						return true;

					case SDLK_F10:
						{
#ifdef _DEBUG
							iGA->MessageBoard()->AddMessage("Screen capture disabled in DEBUG version. Sorry!");
#else
							char *tmp=iGA->GGI()->ScreenCapture(); 
							ASSERT(_snprintf(text,256,"%s saved",tmp)>0);
							iGA->MessageBoard()->AddMessage(text);
							delete tmp;
#endif
							return true;
						}
						
					case SDLK_F7:
					case SDLK_F8:

						if (event.key.keysym.sym==SDLK_F8)
							iGA->GGI()->GD()->NextRes( iGA->Options()->Data().iInGameVideoMode );
						else 
							iGA->GGI()->GD()->PrevRes( iGA->Options()->Data().iInGameVideoMode );
				
						iGA->GGI()->UpdateMode();
						return true;
						
					case SDLK_RETURN:
						if (iGA->EventHandler()->State(SDLK_LALT))
						{
							iGA->Options()->Data().iFullScreen=!iGA->Options()->Data().iFullScreen;
							iGA->GGI()->UpdateMode();
							iGA->EventHandler()->State(SDLK_LALT)=0;
							return true;
						}
						break;
					}
		}
	return false;
}
