#include "CGameApp.h"
#include "CSplash.h"

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

/* chdir to TK directory */
void ChAppDir(const char *argv0)
{	char *str;
	int a;

	str=strdup(argv0);
	a=strlen(str);

	do
	{ 
		a--; 
	} 
#ifndef __LINUX__
	while (str[a]!='\\');
#else
	while (str[a]!='/' || a==0);
#endif

	str[a]=0;

	ChangeDir(str);

	free(str);
}

// TODO: CONFIGFILE
const char* KIconFile="tk.ico";
const char* KWindowCaption="Tapan Kaikki Bloodshed";

int main(int argc,char *argv[]) 
{
	CGameApp* GGameApp = NULL;
	atexit(ExitHandler);
	//ChAppDir(argv[0]);

	try
	{
		// TODO: CONFIGFILE
		CSplash::ShowSplash("efps/splash.bmp",KIconFile,KWindowCaption);
		GGameApp = new CGameApp(KIconFile,KWindowCaption);
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

