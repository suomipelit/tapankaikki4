#include "EditorLevelIO.h"

#include "../common/CEditableLevel.h"
#include "../common/CGraphicsDevice.h"
#include "../common/error.h"
#include "../common/files.h"
#include "EditorDrawer.h"
#include "EditorGraphics.h"

#include <windows.h>
#include <commdlg.h>

CEditorLevelIO::CEditorLevelIO(CEditableLevel* aLevel,SDL_SysWMinfo aWndInfo,CEditorDrawer* aDrawer, CEditorGraphics* aGraphics, CGraphicsDevice* aGD)
{
	iLevel = aLevel;
	iDrawer = aDrawer;
	iWndInfo = aWndInfo;
	iGraphics = aGraphics;
	iGD = aGD;
}

CEditorLevelIO::~CEditorLevelIO(void)
{
}

bool CEditorLevelIO::LoadINI()
{
	OPENFILENAME ofn;       // common dialog box structure
	char szFile[FILENAME_MAX];       // buffer for file name
	char tmp[FILENAME_MAX];
	HWND hwnd;              // owner window
	int ret;

	hwnd = iWndInfo.window;

	_getcwd(tmp,FILENAME_MAX);
	
	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ZeroMemory(&szFile, sizeof(szFile));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "Level (*.ini)\0*.INI\0All (*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

// Display the Open dialog box. 

	if (GetOpenFileName(&ofn)==TRUE)
	{
		_chdir( tmp );
		try
		{
			std::string temp = getpath(ofn.lpstrFile);
			temp += "default.ini";
			iGraphics->Load(temp.c_str(),ofn.lpstrFile);
			iGD->SetPalette(*iGraphics->Palette(),256);
		}
		catch ( CFailureException& e )
		{
			error("Cannot open file (%s)!",e.what());
		}

		return true;
	}
	else
	{
		// TODO: Do nothing 
		ret = CommDlgExtendedError();
	}
	_chdir( tmp );
	return false; // if no INI loaded
}

bool CEditorLevelIO::LoadLevel()
{
	OPENFILENAME ofn;       // common dialog box structure
	char szFile[FILENAME_MAX];       // buffer for file name
	char tmp[FILENAME_MAX];
	HWND hwnd;              // owner window
	int ret;

	hwnd = iWndInfo.window;

	_getcwd(tmp,FILENAME_MAX);
	
	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ZeroMemory(&szFile, sizeof(szFile));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "Level (*.lev)\0*.LEV\0All (*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

// Display the Open dialog box. 

	if (GetOpenFileName(&ofn)==TRUE)
	{
		_chdir( tmp );
		try
		{
			iLevel->Load(ofn.lpstrFile);
			std::string temp = getpath(ofn.lpstrFile);
			temp += "default.ini";
			std::string fname (ofn.lpstrFile);
			fname = fname.substr(0,fname.size()-4)+".ini";
			iGraphics->Load(temp.c_str(),fname.c_str());
			iGD->SetPalette(*iGraphics->Palette(),256);
		}
		catch ( CFailureException& e )
		{
			error("Cannot open file (%s)!",e.what());
		}

		return true;
	}
	else
	{
		// TODO: Do nothing 
		ret = CommDlgExtendedError();
	}
	_chdir( tmp );
	return false; // if no level loaded
}


bool CEditorLevelIO::SaveLevelAs()
{
	OPENFILENAME ofn;       // common dialog box structure
	char szFile[FILENAME_MAX];       // buffer for file name
	char tmp[FILENAME_MAX];
	HWND hwnd;              // owner window
	int ret;

	hwnd = iWndInfo.window;

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ZeroMemory(&szFile, sizeof(szFile));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "Level (*.lev)\0*.LEV\0All (*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_EXPLORER;

// Display the Open dialog box.
	_getcwd(tmp,FILENAME_MAX);

	if (GetSaveFileName(&ofn)==TRUE)
	{
		_chdir( tmp );
		char *tmp;

		if (stricmp(ofn.lpstrFile+strlen(ofn.lpstrFile)-4,".lev")!=0)
		{
			tmp=(char*)malloc(strlen(ofn.lpstrFile)+5);
			sprintf(tmp,"%s.lev",ofn.lpstrFile);
		}
		else
			tmp=strdup(ofn.lpstrFile);

		ASSERT( tmp );

		if (!iLevel->Save( tmp )) 
			error("Saving Failed (%s)!", tmp);
		else 
		{
			free(tmp);
			return true;
		}
	}
	else
	{
		// TODO: Do nothing 
		ret = CommDlgExtendedError();
	}

	_chdir( tmp );
	return false; // If no level saved
}


bool CEditorLevelIO::SaveLevel()
{
	if (*iLevel->LevelFileName() == 0)
		return SaveLevelAs();
	else 
	{
		if (! iLevel->Save(iLevel->LevelFileName()))
			error("Saving Failed (%s)!",iLevel->LevelFileName());
		else
			MessageBox(iWndInfo.window,"Level saved !","",MB_OK | MB_ICONINFORMATION);

		return false;
	}
}
