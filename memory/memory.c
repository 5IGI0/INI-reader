#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../INI_structs.h"
#include "../INI_memory.h"

INI_manager *INI_initManager() {
    return calloc(1, sizeof(INI_manager));
}

void INI_freeManager(INI_manager *manager) {

    for (size_t i = 0; i < manager->size; i++) {
        INI_freeSection(manager->sections[i]);
    }
    
    free(manager->sections);
    free(manager);
}

INI_section *INI_createSection(char *name) {
    INI_section *section = calloc(1, sizeof(INI_section));

    if (section == NULL)
        return NULL;

    section->size = 0;
    section->name = calloc(strlen(name)+1, 1);

    if (section->name == NULL) {
        free(section);
        return NULL;
    }

    strcpy(section->name, name);
    
    return section;
}

_Bool INI_addSection(INI_manager *manager, INI_section *section) {
    
    INI_section **oldAddr = manager->sections;

    manager->sections = realloc(manager->sections, sizeof(INI_section*)*(manager->size+1));
    if (manager->sections == NULL) {
        manager->sections = oldAddr;
        return false;
    }

    manager->size += 1;
    manager->sections[manager->size-1] = section;

    return true;
}

INI_section *INI_getSection(INI_manager *manager, char *name, _Bool create) {
    for (size_t i = 0; i < manager->size; i++) {
        if (!strcmp(name, manager->sections[i]->name)) {
            return manager->sections[i];
        }
    }
    
    if (create) {
        INI_section *section = INI_createSection(name);
        if (section == NULL)
            return NULL;
        
        if(!INI_addSection(manager, section)) {
            free(section);
            return NULL;
        }

        return section;

    }
    
    return NULL;
}

void INI_freeSection(INI_section *section) {

    for (size_t i = 0; i < section->size; i++) {
        free(section->names[i]);
        free(section->values[i]);
    }
    
    free(section->name);
    free(section->values);
    free(section->names);
    free(section);
}

void INI_removeSection(INI_manager *manager, char *name) {
    for (size_t i = 0; i < manager->size; i++) {
        if (!strcmp(manager->sections[i]->name, name)) {
            INI_freeSection(manager->sections[i]);
            manager->size -= 1;

            for (size_t y = i; y < manager->size; y++)
                manager->sections[y] = manager->sections[y+1];

            if (manager->size == 0) {
                free(manager->sections);
                manager->sections = NULL;
            } else
                manager->sections = realloc(manager->sections, sizeof(INI_section*)*manager->size);

            break;
        }
    }
}

void INI_removeValue(INI_manager *manager, char *sectionName, char *key) {
    INI_section *section = INI_getSection(manager, sectionName, false);

    if (section == NULL)
        return;
    
    for (size_t i = 0; i < section->size; i++) {
        if (!strcmp(section->names[i], key)) {
            free(section->names[i]);
            free(section->values[i]);

            section->size -= 1;

            for (size_t y = i; y < section->size; y++) {
                section->names[y] = section->names[y+1];
                section->values[y] = section->values[y+1];
            }

            if (section->size == 0) {
                free(section->values);
                free(section->names);
                section->values = NULL;
                section->names = NULL;
            } else {
                section->names = realloc(section->names, sizeof(char*)*manager->size);
                section->values = realloc(section->values, sizeof(char*)*manager->size);
            }
            break;
        }
    }
    
}