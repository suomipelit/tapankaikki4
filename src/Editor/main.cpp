#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include "editorapp.h"
#include "../common/defines.h"

int main(int argc, char **argv)
{
	CEditorApp* EA;

	EA = new CEditorApp();
	EA->Run();

	delete EA;

	return 1;
}
