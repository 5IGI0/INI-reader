#ifndef __INI_STRUCTS_H__
#define __INI_STRUCTS_H__

#include <stdint.h>

typedef struct __INI_section {
    char **names;
    char **values;
    char *name;
    uint8_t size;
} INI_section;

typedef struct __INI_manager {
    INI_section **sections;
    uint8_t size;
} INI_manager;


#endif