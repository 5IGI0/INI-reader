#include <stdio.h>
#include <stdbool.h>

#include "../INI_structs.h"
#include "../INI_write.h"

_Bool INI_writeFile(INI_manager *manager, char *filename) {
    FILE *file = fopen(filename, "w");

    if (file == NULL)
        return false;

    _Bool tmp = INI_writeFilePointer(manager, file);
    
    return tmp;
}

_Bool INI_writeFilePointer(INI_manager *manager, FILE *file) {
    for (size_t i = 0; i < manager->size; i++) {
        if(fprintf(file, "[%s]\r\n", manager->sections[i]->name) < 0) return false;
        for (size_t y = 0; y < manager->sections[i]->size; y++) {
            if(fprintf(file, "%s=%s\r\n", manager->sections[i]->names[y], manager->sections[i]->values[y]) < 0) return false;;
        }
        
        if(fprintf(file, "\r\n\r\n") < 0) return false;
    }
    return true;
}