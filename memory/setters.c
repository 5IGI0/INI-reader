#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "../INI_memory.h"
#include "../INI_structs.h"

#define FORMAT_BUFFER 25

// TODO : delete the section and realloc names/values in allocation error case
_Bool INI_setValue(INI_manager *manager, char *section, char *key, char *value) {
    char **oldNames = NULL;
    char **oldValues = NULL;
    INI_section *sectionPtr = INI_getSection(manager, section, true);

    if (section == NULL)
        return false;

    for (size_t i = 0; i < sectionPtr->size; i++) {
        if (!strcmp(sectionPtr->names[i], key)) {
            
            oldNames = (void *)sectionPtr->values[i];

            sectionPtr->values[i] = calloc(strlen(value)+1, 1);
            if (sectionPtr->values[i] == NULL) {
                sectionPtr->values[i] = (void *)oldNames;
                return false;
            }
            
            strcpy(sectionPtr->values[i], value);

            return true;
        }
        
    }
    
    
    oldNames = sectionPtr->names;
    oldValues = sectionPtr->values;

    sectionPtr->names = realloc(sectionPtr->names, sizeof(char*)*(sectionPtr->size+1));
    sectionPtr->values = realloc(sectionPtr->values, sizeof(char*)*(sectionPtr->size+1));

    if (sectionPtr->names == NULL || sectionPtr->values == NULL) {
        if (sectionPtr->names == NULL)
            sectionPtr->names = oldNames;
        if (sectionPtr->values == NULL)
            sectionPtr->values = oldValues;
        
        return false;
    }

    sectionPtr->size += 1;

    sectionPtr->names[sectionPtr->size-1] = calloc(strlen(key)+1, 1);
    sectionPtr->values[sectionPtr->size-1] = calloc(strlen(value)+1, 1);

    if (sectionPtr->names[sectionPtr->size-1] == NULL || sectionPtr->values[sectionPtr->size-1] == NULL) {
        sectionPtr->size -= 1;
        return false;
    }

    memcpy(sectionPtr->names[sectionPtr->size-1], key, strlen(key));
    memcpy(sectionPtr->values[sectionPtr->size-1], value, strlen(value));

    return true;
}

_Bool INI_setValueFromInt(INI_manager *manager, char *section, char *key, int value) {
    char buffer[FORMAT_BUFFER] = "";

    if (snprintf(buffer, FORMAT_BUFFER-1, "%d", value) < 0)
        return false;

    return INI_setValue(manager, section, key, buffer);
}