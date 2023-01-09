#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "../headers/headers.h"
#include TERMINAL_H
#include ARGUMENT_H
#include CHOICE_H
#include ERROR_H



int processChoice(struct terminal *trml, struct argument *arg, char choice)
{
    if(choice == ' ' || choice == '\n' || choice == '\r')
    {
        return 0;
    }
    else if(choice == '\033')
    {
        fprintf(stdout, "\n\n");

        set_canon(trml, ON);
        set_echo(trml, ON);

        char string[MAX_INPUTBUFSIZE] = "";

        if(fgets(string, MAX_INPUTBUFSIZE, stdin) == NULL)
        {
            strcpy(originFuncName, "processChoice");
            return -1;
        }

        set_echo(trml, OFF);
        set_canon(trml, OFF);

        long nextPOS;
        if((nextPOS = stringToNum(string)) == -2)
        {
            strcpy(originFuncName, "processChoice");
            errno = ESTRNOTPUREINT;
            return -2;
        }

        int chStrmPosPtrStatus = chStrmPosPtr(arg, nextPOS);
        if(chStrmPosPtrStatus == 0)
        {
            return 1;
        }
        else if(chStrmPosPtrStatus == -1)
        {
            strcpy(originFuncName, "processChoice");
            return -1;
        }
        else if(chStrmPosPtrStatus == -2)
        {
            strcpy(originFuncName, "processChoice");
            errno = EPOSOUTRANGE;
            return -3;
        }
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
        return -2;
    }

    return result;
}


int chStrmPosPtr(struct argument *arg, long nextPOS)
{
    if(nextPOS < arg->filesize)
    {
        nextPOS = nextPOS - nextPOS%arg->column_width;

        if(fseek(arg->srcstream, nextPOS, SEEK_SET) == -1)
        {
            return -1;
        }

        return 0;
    }
    else
    {
        return -2;
    }
}
