#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include "editorapp.h"
#include "../common/defines.h"

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
	while (str[a]!='/');
#endif

	str[a]=0;

	ChangeDir(str);

	free(str);
}

int main(int argc, char **argv)
{
	CEditorApp* EA;

	//ChAppDir( argv[0] );
	EA = new CEditorApp();
	EA->Run();

	delete EA;

	return 1;
}
