#ifndef __CMESSAGEBOARD_H
#define __CMESSAGEBOARD_H

#include <SDL.h>
#include "common/defines.h"
#include "common/CRect.h"
#include "common/CDrawArea.h"
#include "COptions.h"


const int KMaxMessagesOnBoard=6;
const int KMaxMessagesLengthOnBoard=80;
const int KNetMessageMaxLength=(KMaxMessagesLengthOnBoard-KPlayerNameMaxLength-1);

class CGameGraphicsInterface;
class CDrawArea;

class CMessageBoard
{
public:
	CMessageBoard();
	~CMessageBoard();
	void AddMessage(const char* msg);
	void Run(CDrawArea& aDirtyArea);
	void Draw(CGameGraphicsInterface* aGGI,CDrawArea& aDrawArea);
	void Clear();
	void SetDrawOffset( int aOffset ); // vertical offset to messageboard

private:
	CDrawArea iCleanArea;
	bool iNeedUpdate;
	int iOffset;
	int iClearTime;
	std::vector< std::string > iMessages;
    SDL_sem* iSemaphore;
};

#endif


