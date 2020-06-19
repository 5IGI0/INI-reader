#ifndef __INI_STRUCTS_H__
#define __INI_STRUCTS_H__

#include <stdint.h>

typedef struct __INI_value {
    char *name;
    char *value;
} INI_value;

typedef struct __INI_section {
    INI_value *values;
    char *name;
    uint8_t size;
} INI_section;

typedef struct __INI_manager {
    INI_section **sections;
    uint8_t size;
} INI_manager;


#endif