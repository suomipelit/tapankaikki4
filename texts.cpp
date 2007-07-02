#include "CKeys.h"
#include "texts.h"

// TODO: CONFIGFILE
const char *k_text[EKeyAmount] = 
{
	"turn c.clockw.", 
	"turn clockw.", 
	"walk forward", 
	"walk backward", 
	"fire", 
	"change weapon", 
	"strafe", 
	"strafe left", 
	"strafe right", 
};

extern const int KAmountMusicFilenameExtensions = 5;
const char *KMusicFilenameExtensions[] =
{
	".s3m",
	".xm",
	".ogg",
	".it",
	".mp3",
};

const char *KOptionsFilename="options.cfg";

const int KBoolCatMax=3; // Length of maximum string in KBoolCatStr
const char *KBoolCatStr[2] =
{
	"off",
	"on"
};

const char *KQuitConfirmText = "Quit TK? (Y/N)";
const char *KDefaultMusicTheme="Default";

