#ifndef __INI_WRITE_H__
#define __INI_WRITE_H__

#include <stdbool.h>

#include "INI_structs.h"

_Bool INI_writeFilePointer(INI_manager *manager, FILE *file);
_Bool INI_writeFile(INI_manager *manager, char *filename);

#endif