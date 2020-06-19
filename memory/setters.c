#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "../INI_memory.h"
#include "../INI_structs.h"

#define FORMAT_BUFFER 25

// TODO : delete the section and realloc names/values in allocation error case
_Bool INI_setValue(INI_manager *manager, char *section, char *key, char *value) {
    INI_value *oldValues = NULL;
    INI_section *sectionPtr = INI_getSection(manager, section, true);

    if (section == NULL)
        return false;

    for (size_t i = 0; i < sectionPtr->size; i++) {
        if (!strcmp(sectionPtr->values[i].name, key)) {
            char *tmp = NULL;
            
            tmp = sectionPtr->values[i].value;

            sectionPtr->values[i].value = calloc(strlen(value)+1, 1);
            if (sectionPtr->values[i].value == NULL) {
                sectionPtr->values[i].value = tmp;
                return false;
            }
            
            strcpy(sectionPtr->values[i].value, value);

            return true;
        }
        
    }
    
    oldValues = sectionPtr->values;

    sectionPtr->values = realloc(sectionPtr->values, sizeof(INI_value)*(sectionPtr->size+1));

    if (sectionPtr->values == NULL) {
        sectionPtr->values = oldValues;
        return false;
    }

    sectionPtr->size += 1;

    sectionPtr->values[sectionPtr->size-1].name = calloc(strlen(key)+1, 1);
    sectionPtr->values[sectionPtr->size-1].value = calloc(strlen(value)+1, 1);

    if (sectionPtr->values[sectionPtr->size-1].name == NULL || sectionPtr->values[sectionPtr->size-1].value == NULL) {
        sectionPtr->size -= 1;
        return false;
    }

    memcpy(sectionPtr->values[sectionPtr->size-1].name, key, strlen(key));
    memcpy(sectionPtr->values[sectionPtr->size-1].value, value, strlen(value));

    return true;
}

_Bool INI_setValueFromInt(INI_manager *manager, char *section, char *key, int value) {
    char buffer[FORMAT_BUFFER] = "";

    if (snprintf(buffer, FORMAT_BUFFER-1, "%d", value) < 0)
        return false;

    return INI_setValue(manager, section, key, buffer);
}