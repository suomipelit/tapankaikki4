#ifndef __EDITORLEVELIO_H
#define __EDITORLEVELIO_H
#include <SDL_syswm.h>

class CEditableLevel;
class CEditorDrawer;
class CEditorGraphics;
class CGraphicsDevice;

class CEditorLevelIO
{
public:
	CEditorLevelIO(CEditableLevel* aLevel,SDL_SysWMinfo aWndInfo,CEditorDrawer* aDrawer, CEditorGraphics* aGraphics, CGraphicsDevice* aGD);
	~CEditorLevelIO(void);
	bool LoadINI();
	bool LoadLevel();
	bool SaveLevelAs();
	bool SaveLevel();
	
protected:
	CEditableLevel* iLevel;
	CEditorGraphics* iGraphics;
	CGraphicsDevice* iGD;
	CEditorDrawer* iDrawer;
	SDL_SysWMinfo iWndInfo;
	
};

#endif
