# INI reader/writer

Library for reading/writing .ini.

## TODO 

- [ ] document the code
- [ ] fragment files
- [ ] get value as int
- [ ] set value from int
- [ ] dump to string

## Example

```c
#include <stdio.h>
#include <stdlib.h>

#include "INI.h"

int main(void) {
    INI_manager *iniManager = INI_initManager();
    char *username = NULL;

    if (!iniManager)
        exit(EXIT_FAILURE);

    INI_readFile("user.ini", iniManager);

    username = INI_getValue(iniManager, "user", "name");

    if (username != NULL) {
        printf("Welcome %s !\n", username);
    } else {
        char toSet[25] = "";
        printf("What's your name ?");
        scanf("%24s", toSet);
        INI_setValue(iniManager, "user", "name", toSet);
    }

    INI_writeFile(iniManager, "user.ini");
    
    INI_freeManager(iniManager);
    
    return 0;
}

```