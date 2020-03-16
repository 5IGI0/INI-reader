#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "INI_structs.h"
#include "INI_memory.h"

typedef enum {
    INI_STAGE_SECTION,
    INI_STAGE_SECTION_END,
    INI_STAGE_ENTRY_KEY,
    INI_STAGE_ENTRY_VALUE,
    INI_STAGE_EMPTYLINE
} INI_stage;

_Bool INI_readFile(char *filename, INI_manager *manager) {
    FILE *file = fopen(filename, "rb");
    size_t currentLineOffset = 0;
    char *currentSection = NULL;

    if (!file)
        return false;
    
    while (true) {

        if (fgetc(file) != '\n') {
            INI_stage stage = INI_STAGE_EMPTYLINE;
            int currentChar = 0;
            size_t keySize = 0;
            char *keyValue = NULL;
            size_t valueSize = 0;
            char *valueValue = NULL;
            size_t tmpSpace = 0;
            fseek(file, currentLineOffset, SEEK_SET);

            // size calculation ...
            for (size_t i = 0; (currentChar = fgetc(file)) != '\n' && currentChar != '\r'; i++) {

                if (currentChar == EOF)
                    break;

                if (i == 0 && currentChar == '[') {
                    stage = INI_STAGE_SECTION;
                    continue;
                } else if (i == 0) {
                    stage = INI_STAGE_ENTRY_KEY;
                }
                
                if (stage == INI_STAGE_ENTRY_KEY) {// tmpSpace
                    if ((currentChar == ' ' || currentChar == '\t')) {
                        if (keySize == 0)
                            continue;
                        else
                            tmpSpace += 1;
                    } else {
                        if (currentChar == '=')
                            stage = INI_STAGE_ENTRY_VALUE;
                        else
                            keySize += 1+tmpSpace;
                        tmpSpace = 0;
                    }
                } else if (stage == INI_STAGE_ENTRY_VALUE) {// tmpSpace
                    if ((currentChar == ' ' || currentChar == '\t')) {
                        if (valueSize == 0)
                            continue;
                        else
                            tmpSpace += 1;
                    } else {
                        valueSize += 1+tmpSpace;
                        tmpSpace = 0;
                    }
                } else if (stage == INI_STAGE_SECTION) {
                    if (currentChar == ']') {
                        stage == INI_STAGE_SECTION_END;
                        continue;
                    }
                    keySize += 1;
                }
                
            }

            tmpSpace = 0; // needed for setting keys/values

            fseek(file, currentLineOffset, SEEK_SET);

            if (stage == INI_STAGE_SECTION_END|| stage == INI_STAGE_SECTION) {
                free(currentSection);
                currentSection = calloc(keySize+1, 1);
                stage = INI_STAGE_SECTION;
            } else if (stage == INI_STAGE_ENTRY_KEY)
                stage = INI_STAGE_EMPTYLINE; // ingore entry without value
            else if (INI_STAGE_ENTRY_VALUE) {
                if (valueSize == 0 || keySize == 0)
                    stage = INI_STAGE_EMPTYLINE; // ingore entry without value or key
                else {
                    stage = INI_STAGE_ENTRY_KEY;

                    keyValue = calloc(keySize+1, 1);
                    valueValue = calloc(valueSize+1, 1);

                    if (valueValue == NULL || keyValue == NULL) {
                        stage = INI_STAGE_EMPTYLINE; // don't read in allocation failure case

                        if (valueValue != NULL) {
                            free(valueValue);
                            valueValue = NULL;
                        }
                        
                        if (keyValue != NULL) {
                            free(keyValue);
                            keyValue = NULL;
                        }
                    }
                }
            }
            
            

            for (size_t i = 0; (currentChar = fgetc(file)) != '\n' && currentChar != '\r'; i++) {
                if (currentChar == EOF)
                    break;
                
                if (stage == INI_STAGE_SECTION && currentSection != NULL) {
                    if (i == 0)
                        continue;
                    if (currentChar == ']') {
                        stage = INI_STAGE_SECTION_END;
                        continue;
                    }
                    currentSection[i-1] = currentChar;
                } else if (stage == INI_STAGE_ENTRY_KEY) {
                    if (currentChar == ' ' && tmpSpace != 0 && tmpSpace < keySize) {
                        keyValue[tmpSpace] = ' ';
                        tmpSpace += 1;
                    } else if(currentChar != ' ' && tmpSpace < keySize) {
                        keyValue[tmpSpace] = currentChar;
                        tmpSpace += 1;
                    } else if(currentChar == '=') {
                        stage = INI_STAGE_ENTRY_VALUE;
                        tmpSpace = 0;
                    }
                } else if (stage == INI_STAGE_ENTRY_VALUE) {
                    if (currentChar == ' ' && tmpSpace != 0 && tmpSpace < valueSize) {
                        valueValue[tmpSpace] = ' ';
                        tmpSpace += 1;
                    } else if(currentChar != ' ' && tmpSpace < valueSize) {
                        valueValue[tmpSpace] = currentChar;
                        tmpSpace += 1;
                    }
                }
                
            }

            // printf("[%ld:%ld:%u] %s.%s = %s\n", valueSize, keySize, stage, currentSection, keyValue, valueValue);

            if (keyValue != NULL && valueValue != NULL && currentSection != NULL) {
                INI_setValue(manager, currentSection, keyValue, valueValue);
            }
            

            free(keyValue);
            keyValue = NULL;
            free(valueValue);
            valueValue = NULL;
        }
        
        currentLineOffset = ftell(file);
        if (fgetc(file) == '\n') // ignore \r
            currentLineOffset += 1;
        else
            fseek(file, currentLineOffset, SEEK_SET);

        
        if (fgetc(file) == EOF)
            break;
        else
            fseek(file, currentLineOffset, SEEK_SET);
    }

    free(currentSection);

    return true;
}