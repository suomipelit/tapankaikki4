#include "CWebVersionChecker.h"
#include "CMessageBoard.h"
#include "CGameVersion.h"

int CWebVersionChecker::WebVersionCheckerObserver(void * obj) 
{
    CWebVersionChecker* WVC = (CWebVersionChecker*) obj;
    WVC->Run();
	return 0;
}

CWebVersionChecker::CWebVersionChecker(CMessageBoard* aMB, CNetworkDevice* aND, COptions* aOptions): iMB(aMB), iND(aND), iOptions(aOptions), iThread(NULL)
{
}

CWebVersionChecker::~CWebVersionChecker()
{
	Kill();
}

void CWebVersionChecker::Kill()
{
	if (iThread)
	{
		SDL_KillThread(iThread);
		iThread=NULL;
	}
}

void CWebVersionChecker::Start()
{
	iThread=SDL_CreateThread(WebVersionCheckerObserver,this);
}

void CWebVersionChecker::Run()
{
	int error;
	char *buf;
	char *version;
	char *name;
	char *url;
	char *tmp;

	if (iOptions->Data().iProxyEnabled)
	{
		if (atoi(iOptions->Data().iProxyPort)<=0||
			atoi(iOptions->Data().iProxyPort)>0xffff)
			iMB->AddMessage("Web Version Check failed: Illegal proxy port!");
		
		if (strlen(iOptions->Data().iProxyAddr)<=0)
			iMB->AddMessage("Web Version Check failed: Illegal proxy addr!");

		buf=iND->ReadProxyHTTP(iOptions->Data().iProxyAddr,KWEBCHECKHOST,KWEBCHECKPAGE,atoi(iOptions->Data().iProxyPort),&error);
	}
	else 
		buf=iND->ReadHTTP(KWEBCHECKHOST,KWEBCHECKPAGE,80,&error);

	if (error>0&&error<=2)
	{
		LOG1("CWebVersionChecker::Run(): Version check skipped (%s)",buf);
		free(buf);
	}
	else
		if (error>2)
		{
			iMB->AddMessage(buf);
			free(buf);
		}
		else
		{
			version=strstr(buf,"version=");
			name=strstr(buf,"name=");
			url=strstr(buf,"url=");

			if (version==NULL)
			{
				iMB->AddMessage("Web Version Check failed: Version number not found in response");
				return;
			}
			if (name==NULL)
			{
				iMB->AddMessage("Web Version Check failed: Version name not found in response");
				return;
			}

			if (url==NULL)
			{
				iMB->AddMessage("Web Version Check failed: Download url not found in response");
				return;
			}

			version+=strlen("version=");
			name+=strlen("name=");
			url+=strlen("url=");

			tmp=strchr(version,'\n');if (tmp) tmp[0]=0;tmp=strchr(version,'\r');if (tmp) tmp[0]=0;
			tmp=strchr(name,'\n');if (tmp) tmp[0]=0;tmp=strchr(name,'\r');if (tmp) tmp[0]=0;
			tmp=strchr(url,'\n');if (tmp) tmp[0]=0;tmp=strchr(url,'\r');if (tmp) tmp[0]=0;

			if (CGameVersion::Compare(version)>0)
			{
				iMB->AddMessage("TK is NOT up-to-date!");
				iMB->AddMessage(name);
				iMB->AddMessage("now available at ");
				iMB->AddMessage(url);
			}
			else
				if (CGameVersion::Compare(version)<0)
				{
					iMB->AddMessage("Funny. You seem to have newer TK than the official website...");
				}
		}
}



