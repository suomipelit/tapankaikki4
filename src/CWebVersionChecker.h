#ifndef __WEBVERSIONCHECKER_H_
#define __WEBVERSIONCHECKER_H_

#include <SDL.h>
#include <SDL_thread.h>

class CNetworkDevice;
class CMessageBoard;
class COptions;

#define KWEBCHECKHOST "www.errorfree.eu.org"
#define KWEBCHECKPAGE "/tk.version"

class CWebVersionChecker
{
public:
	CWebVersionChecker(CMessageBoard* aMB, CNetworkDevice* aND, COptions* aOptions);
	~CWebVersionChecker();
	void Start();
	void Kill();
	static int WebVersionCheckerObserver(void *obj);

protected:
	void Run();

	SDL_Thread* iThread;
	CNetworkDevice* iND;
	CMessageBoard* iMB;
	COptions* iOptions;

};

#endif
