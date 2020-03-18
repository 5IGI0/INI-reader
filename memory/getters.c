#include <stdlib.h>

#include "../INI_structs.h"

char *INI_getValue(INI_manager *manager, char *section, char *name) {
    for (size_t i = 0; i < manager->size; i++) {
        if (!strcmp(manager->sections[i]->name, section)) {
            for (size_t y = 0; y < manager->sections[i]->size; y++) {
                if (!strcmp(manager->sections[i]->names[y], name)) {
                    return manager->sections[i]->values[y];
                }
            }
        }
    }
    return NULL;
}

