#ifndef __FILES_H
#define __FILES_H 

#include "defines.h"
#include <string>

// FILESYSTEM DEFINES
const int ESUBDIR=0;
const int EFILE=1;

EXPORT std::string getsavebasedir();
EXPORT std::string getsavepath(const std::string& name);
EXPORT std::string getdatapath(const std::string& name);
EXPORT int exists(const char *name);
EXPORT int filelen(const char *name);
EXPORT int filetimestamp(const char *name);
EXPORT std::string getpath(const char *name);

#endif





