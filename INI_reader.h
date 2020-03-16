#ifndef __INI_READER_H__
#define __INI_READER_H__

#include <stdbool.h>

#include "INI_structs.h"
#include "INI_reader.h"

_Bool INI_readFile(char *filename, INI_manager *manager);

#endif