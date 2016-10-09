#include <vector>
#include "common/CFonts.h"
#include "common/error.h"

#include "CMessageBoard.h"
#include "CGameGraphicsInterface.h"
#include "timer.h"

CMessageBoard::CMessageBoard()
{
	iOffset = 0;
	iSemaphore=SDL_CreateSemaphore(1);
}

CMessageBoard::~CMessageBoard()
{
	Clear();
	SDL_DestroySemaphore(iSemaphore);
}

void CMessageBoard::AddMessage(const char* msg)
{
	std::string str=msg;
	LOG1("message board: %s\n", msg);

	if (SDL_SemWait(iSemaphore) == -1)
	{
        return;
	}

	if (str.length()>KMaxMessagesLengthOnBoard)
	{
		LOG1("CMessageBoard::AddMessage: Too long message: %s", str.c_str());
		str = str.substr(0,KMaxMessagesLengthOnBoard-3);
		str += "...";
	}

	iMessages.push_back( str );

	while (iMessages.size() > KMaxMessagesOnBoard)
	{
		iMessages.erase( iMessages.begin() );
	}
	iClearTime=TimerCounter+KMessageTimeOnScreen;
	iNeedUpdate = true;
	SDL_SemPost(iSemaphore);
}

void CMessageBoard::Draw(CGameGraphicsInterface* aGGI,CDrawArea& aDrawArea)
{
	const int yoffs=5+iOffset;
	const int lineHeight = aGGI->Font(CGameGraphicsInterface::ESmallFont)->Height() + 1;
	
	iCleanArea.Reset();

	if (SDL_SemWait(iSemaphore) == -1)
	{
        return;
	}

	for (int a=0;a<iMessages.size(); a++)
		iCleanArea.Combine(aGGI->Font(CGameGraphicsInterface::ESmallFont)->
			Write(5,yoffs+a*lineHeight,iMessages[a].c_str(),CFonts::EDrawBelow, CFonts::EDrawToRight,aGGI->DrawBuffer()));

	SDL_SemPost(iSemaphore);

	aDrawArea.Combine(iCleanArea);
}

void CMessageBoard::Run(CDrawArea& aDirtyArea)
{
	if ( iClearTime< TimerCounter )
	{
		if (iMessages.size())
			Clear();
	}

	if (iNeedUpdate)
		aDirtyArea.Combine(iCleanArea);
	iNeedUpdate = false;
}

void CMessageBoard::Clear()
{
	if (SDL_SemWait(iSemaphore) == -1)
	{
        return;
	}

	iMessages.clear();
	SDL_SemPost(iSemaphore);

	iNeedUpdate = true;
}

	
void CMessageBoard::SetDrawOffset( int aOffset )
{
	ASSERT( aOffset >= 0 );
	ASSERT( aOffset < 1000 );
	iOffset = aOffset;
}
