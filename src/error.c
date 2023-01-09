#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "../headers/headers.h"
#include ERROR_H

char originFuncName[MAX_FUNCNAMELENGTH] = "main";

void printerror()
{
    if(errno == EINVALFLAGS)
    {
        fprintf(stderr, "Error: %s->  Invalid flags\n", originFuncName);
    }
    else if(errno == EINVCOLWIDTHMOD)
    {
        fprintf(stderr, "Error: %s->  Invalid value of column_width modifier\n", originFuncName);
    }
    else if(errno == EPOSOUTRANGE)
    {
        fprintf(stderr, "Error: %s->  Entered Offset value is beyond EOF\n", originFuncName);
    }
    else if(errno == ESTRNOTPUREINT)
    {
        fprintf(stderr, "Error: %s->  Entered string contains invalid character(s)\n", originFuncName);
    }
    else if(errno == EFLAGSREPEATED)
    {
        fprintf(stderr, "Error: %s->  Flag(s) repeated in the passed arguments\n", originFuncName);
    }
    else
    {
        fprintf(stderr, "Error: %s->  %s\n", originFuncName, strerror(errno));
    }
}
