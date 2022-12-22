#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "terminal1.h"
#include "argument1.h"
#include "choice1.h"
#include "error1.h"


/*
 *  return -2   -> Error No = EPOSOUTRANGE or ESTRNOTPUREINT, user has requested for for jump to New POS but entered value is Invalid
 *  return -1   -> Error No = |||varies|||, fatal error, occured in a Standard library function
 *  return 0    -> No Error, user has requested for Next set of output
 *  return 1    -> No Error, user has requested to jump to New POS and New POS is initialised
 *  return 2    -> No Error, user has requested for exit of the program
 *  return 3    -> No Error, user has entered Invalid choice
 */
int processChoice(struct terminal *trml, struct argument *arg, char choice)
{
    strcpy(originFuncName, "processChoice");

    if(choice == ' ' || choice == '\n' || choice == '\r')
    {
        return 0;
    }
    else if(choice == '\033')
    {
        set_canon(trml, ON);
        set_echo(trml, ON);

        char string[MAX_INPUTBUFSIZE] = "";

        if(fgets(string, MAX_INPUTBUFSIZE, stdin) == NULL)
        {
            return -1;
        }

        set_echo(trml, OFF);
        set_canon(trml, OFF);

        long nextPOS;
        if((nextPOS = stringToNum(string)) == -1)
        {
            return -2;
        }

        int chStrmPosPtrStatus = chStrmPosPtr(arg, nextPOS);
        if(chStrmPosPtrStatus == 0) return 1;
        else if(chStrmPosPtrStatus == -1) return -1;
        else if(chStrmPosPtrStatus == -2) return -2;
    }
    else if(choice == 'q' || choice == 'Q' || choice == EOF)
    {
        return 2;
    }
    else
    {
        return 3;
    }
}


long stringToNum(char *string)
{
    char *endptr;
    long result = strtol(string, &endptr, 0);
    if(!((*string != '\0' || *string != '\n' || *string != '\r') && (*endptr == '\0' || *endptr == '\n' || *endptr == '\r')))
    {
        errno = ESTRNOTPUREINT;
        return -1;
    }

    return result;
}


int chStrmPosPtr(struct argument *arg, long nextPOS)
{
    if(nextPOS < arg->filesize)
    {
        int initialPOS = ftell(arg->srcstream);
        nextPOS = nextPOS - nextPOS%arg->column_width;

        if(fseek(arg->srcstream, nextPOS, SEEK_SET) == -1)
        {
            return -1;
        }

        return 0;
    }
    else
    {
        errno = EPOSOUTRANGE;
        return -2;
    }
}
