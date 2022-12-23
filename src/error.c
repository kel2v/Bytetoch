#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "../headers/headers.h"
#include ERROR_H

void printerror()
{
    if(errno == EINVARGCOUNT)
    {
        fprintf(stderr, "Error: %s\tInvalid number of arguments\n", originFuncName);
    }
    else if(errno == EINVCOLWIDTHMOD)
    {
        fprintf(stderr, "Error: %s\tInvalid value of column_width modifier\n", originFuncName);
    }
    else if(errno == EPOSOUTRANGE)
    {
        fprintf(stderr, "Error: %s\tEntered Offset value is beyond EOF\n", originFuncName);
    }
    else if(errno == ESTRNOTPUREINT)
    {
        fprintf(stderr, "Error: %s\tEntered string contains invalid character(s)\n", originFuncName);
    }
    else
    {
        fprintf(stderr, "Error: %s\t%s\n", originFuncName, strerror(errno));
    }
}
