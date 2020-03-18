#ifndef __INI_MEMORY_H__
#define __INI_MEMORY_H__

#include "INI_structs.h"

INI_manager *INI_initManager();
void INI_freeManager(INI_manager *manager);
INI_section *INI_createSection(char *name);
_Bool INI_addSection(INI_manager *manager, INI_section *section);
INI_section *INI_getSection(INI_manager *manager, char *name, _Bool create);
_Bool INI_setValue(INI_manager *manager, char *section, char *key, char *value);
void INI_freeSection(INI_section *section);
void INI_removeSection(INI_manager *manager, char *name);
void INI_removeValue(INI_manager *manager, char *sectionName, char *key);

// getters
char *INI_getValue(INI_manager *manager, char *section, char *name);
_Bool INI_getValueAsInt(INI_manager *manager, char *section, char *name, int *output);

#endif