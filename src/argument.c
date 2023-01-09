#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

#include "../headers/headers.h"
#include ARGUMENT_H
#include ERROR_H


int init_Argument(struct argument *arg, int argc, char *argv[])
{
    arg->srcstream = NULL;

    int validateArgumentsStatus = validateArguments(argc, argv);
    if(validateArgumentsStatus == -2)
    {
        strcpy(originFuncName, "init_Argument");
        errno = EINVALFLAGS;
        return -2;
    }
    else if(validateArgumentsStatus == -3)
    {
        strcpy(originFuncName, "init_Argument");
        errno = EFLAGSREPEATED;
        return -3;
    }

    if(init_src(arg, argc, argv) == -1)
    {
        strcpy(originFuncName, "init_Argument");
        return -1;
    }

    if(init_fileSize(arg) == -1)
    {
        strcpy(originFuncName, "init_Argument");
        return -1;
    }

    init_onlyText(arg, argc, argv);

    if(init_columnWidth(arg, argc, argv) == -2)
    {
        strcpy(originFuncName, "init_Argument");
        errno = EINVCOLWIDTHMOD;
        return -4;
    }

    return 0;
}


int validateArguments(int argc, char *argv[])
{
    int tpresent = 0, cpresent = 0;
    for(int i=1; i<argc-1; i++)
    {
        if(strcmp(argv[i], "-t") == 0)
        {
            if(tpresent)
            {
                return -3;
            }

            tpresent = 1;
        }
        else if(strcmp(argv[i], "-c") == 0)
        {
            if(tpresent)
            {
                return -3;
            }

            ++i;
            cpresent = 1;
        }
        else
        {
            return -2;
        }
    }

    return 0;
}


int init_src(struct argument *arg, int argc, char *argv[])
{
    if(access(argv[argc-1], F_OK) != 0 || (arg->srcstream = fopen(argv[argc-1], "rb")) == NULL)
    {
        return -1;
    }

    return 0;
}


int init_fileSize(struct argument *arg)
{
    int fd;
    if((fd = fileno(arg->srcstream)) == -1)
    {
        return -1;
    }

    struct stat st;
    if(fstat(fd, &st) == -1)
    {
        return -1;
    }

    arg->filesize = st.st_size;

    return 0;
}


int init_onlyText(struct argument *arg, int argc, char *argv[])
{
    arg->onlytext = 0;

    if(argc >= 3)
    {
      for(int i=1; i<argc-1; i++)
      {
        if(strcmp(argv[i], "-t") == 0)
        {
            arg->onlytext = 1;
            return 1;
        }
      }
    }

    return 0;
}


int init_columnWidth(struct argument *arg, int argc, char *argv[])
{
    arg->column_width = DEFAULT_COLWIDTH;

    if(argc >= 3)
    {
        int modifier = 0;

        for(int i=1; i<argc-1; i++)
        {
          if(strcmp(argv[i], "-c") == 0)
          {
            modifier = i+1;
            break;
          }
        }

        if(modifier != 0)
        {
            char *endptr;
            long newColWidthVal = strtol(argv[modifier], &endptr, 10);
            if( !(*argv[modifier] != '\0' && *endptr == '\0') || newColWidthVal < MIN_COLWIDTH || newColWidthVal > MAX_COLWIDTH )
            {
                errno = EINVCOLWIDTHMOD;
                return -2;
            }

            arg->column_width = newColWidthVal;
            return 1;
        }
    }

    return 0;
}
