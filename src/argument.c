#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

#include "../headers/headers.h"
#include ARGUMENT_H
#include ERROR_H


/*
 *  int init_Argument(struct argument *arg, int argc, char *argv[]);
 *
 *  return -2   -> Error No = EINVARGCOUNT, fatal error, Invalid number of arguments
 *  return -1   -> Error No = |||varies|||, fatal error, occured in a Standard library function
 *  return 1    -> Error No = EINVCOLWIDTHMOD, non-fatal error, Invalid value of column_width modifier
 *  return 0    -> NO ERROR
 */
int init_Argument(struct argument *arg, int argc, char *argv[])
{
    strcpy(originFuncName, "init_Argument");

    if(argc <= 1 || argc > 5)
    {
        errno = EINVARGCOUNT;
        return -2;
    }

    arg->srcstream = NULL;
    if(init_src(arg, argc, argv) == -1)
    {
        return -1;
    }

    int invalidColWidthMod = 0;
    if(init_columnWidth(arg, argc, argv) == -2)
    {
        invalidColWidthMod = 1;
    }

    init_onlyText(arg, argc, argv);

    if(init_fileSize(arg) == -1)
    {
        return -1;
    }

    if(invalidColWidthMod) return 1;
    else return 0;
}



int init_src(struct argument *arg, int argc, char *argv[])
{
    if(access(argv[argc-1], F_OK) != 0 || (arg->srcstream = fopen(argv[argc-1], "rb")) == NULL) return -1;
    return 0;
}


/*
 *  int init_columnWidth(struct argument *arg, int argc, char *argv[])
 *
 *  return -2   -> Error No = EINVCOLWIDTHMOD, Indicates that argc is greater than 2 and "-c" is detected but "Invalid value of column_width modifier"
 *  return 1    -> No Error, Indicates that argc is greater than 2 and "-c" is detected and new column_width is successfully set.
 *  return 2    -> No Error, Indicates that argc is greater than 2 but "-c" flag is not detected in arguments
 *  return 0    -> No Error, Indicates that argc is not greater than 2 and hence column_width is assigned with DEFAULT_COLWIDTH
 */
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
        else
        {
            return 2;
        }
    }

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
