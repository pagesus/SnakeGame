#include "get_conf.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define LINE_BUF 100
int getIntConf(FILE *fp ,const char* name, int* val)
{
    int retVal = -1;
    char line[LINE_BUF+1] = {0};
    *val = 0;
    while(fgets(line, LINE_BUF, fp))
    {
        char* strloc = NULL;
        if((strloc=strstr(line, name)))
        {
            *val = strtol(strloc + strlen(name) + 1, NULL, 10);
            retVal = 0;
            break;
        }
    }
    return retVal;
}


