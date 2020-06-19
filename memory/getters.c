#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../INI_structs.h"

char *INI_getValue(INI_manager *manager, char *section, char *name) {
    for (size_t i = 0; i < manager->size; i++) {
        if (!strcmp(manager->sections[i]->name, section)) {
            for (size_t y = 0; y < manager->sections[i]->size; y++) {
                if (!strcmp(manager->sections[i]->values[y].name, name)) {
                    return manager->sections[i]->values[y].value;
                }
            }
        }
    }
    return NULL;
}

_Bool INI_getValueAsInt(INI_manager *manager, char *section, char *name, int *output) {
    char *tmp = INI_getValue(manager, section, name);

    if (tmp == NULL)
        return false;

    output[0] = atoi(tmp);

    if (output[0] == 0) {
        if(tmp[0] == '0') return true;
        else              return false;
    }
    
    return true;
}