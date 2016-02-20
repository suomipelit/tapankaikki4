#include "CGameApp.h"
#include "CSplash.h"

#include "common/files.h"

int abnormal_exit=1;

void ExitHandler()
{
	if (abnormal_exit)
	{
		logwrite("Abnormal exit\n");
#ifdef _DEBUG
		_asm { int 3h }
#endif
	}
}

// TODO: CONFIGFILE
const std::string KIconFile="tk.ico";
const char* KWindowCaption="Tapan Kaikki Bloodshed";

int main(int argc,char *argv[]) 
{
	CGameApp* GGameApp = NULL;
	atexit(ExitHandler);
	chdir(getdatapath(".").c_str());

	try
	{
		// TODO: CONFIGFILE
		CSplash::ShowSplash("efps/splash.bmp",KIconFile.c_str(),KWindowCaption);
		GGameApp = new CGameApp(KIconFile.c_str(),KWindowCaption);
#ifdef _DEBUG
		GGameApp->SelfTest();
#endif
		GGameApp->Run(argc,argv);
		delete GGameApp;
	}
	catch (CCriticalException& e)
	{
		error("CCriticalException: %s",e.what());
	}
	catch (CFailureException& e)
	{
		error("CFailureException: %s",e.what());
	}
	catch (CGameException& e)
	{
		error("CGameException: %s",e.what());
	}
	catch (std::exception& e)
	{
		error("std::exception: %s",e.what());
	}
	
	abnormal_exit=0;
	return 0;
}

