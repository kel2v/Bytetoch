#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "../headers/headers.h"
#include TERMINAL_H
#include ARGUMENT_H
#include CHOICE_H
#include OUTPUT_H
#include ERROR_H
#include CLOSE_H


int main(int argc, char *argv[])
{
    struct terminal trml;
    if(init_terminal(&trml, STDIN_FILENO) == -1)  //has doubt with 'fd' associated with terminal and program's stdin
    {
        printerror();
        return EXIT_FAILURE;
    }


    struct argument arg;
    int initArgumentStatus = init_Argument(&arg, argc, argv);


    if(initArgumentStatus == -1 || initArgumentStatus == -2 || initArgumentStatus == -3)
    {
        printerror();
        closeProgram(&trml, &arg, EXIT_FAILURE);
    }
    else if(initArgumentStatus == -4)
    {
        printerror();
    }


    struct output outputParam;
    init_Output(&outputParam, &arg);


    if(set_canon(&trml, OFF) == -1 || set_echo(&trml, OFF) == -1)
    {
        printerror();
        closeProgram(&trml, &arg, EXIT_FAILURE);
    }


    int dontStop = 1, lastBuffer = 0;
    while(dontStop)
    {
        char choice = fgetc(stdin);

        int processChoiceStatus = processChoice(&trml, &arg, choice);
        if(processChoiceStatus == 1)
        {
            printf("\n");
            lastBuffer = 0;
            resetOutputBuffer(&outputParam);
        }
        else if(processChoiceStatus == 2)
        {
            break;
        }
        else if(processChoiceStatus == 3)
        {
            continue;
        }
        else if(processChoiceStatus == -1)
        {
            printerror();
            closeProgram(&trml, &arg, EXIT_FAILURE);
        }
        else if(processChoiceStatus == -2 || processChoiceStatus == -3)
        {
            printerror();
            printf("\n\n");
            continue;
        }


        /*
            0 && 0 (0):-> not last buffer and buffer is not short
            0 && 1 (1):-> not last buffer and buffer is short
            1 && 0 (0):-> last buffer and buffer is not short
            1 && 1 (0):-> last buffer and buffer is short
        */
        if(!lastBuffer && isShortage(&outputParam))
        {
            int loadBufferStatus = loadOutputBuffer(&outputParam, &arg);
            if(loadBufferStatus == -1)
            {
                printerror();
                closeProgram(&trml, &arg, EXIT_FAILURE);
            }
        }


        if(feof(arg.srcstream) != 0)
        {
            lastBuffer = 1;

            if(isShortage(&outputParam))
            {
                reinit_outputColWidth(&outputParam);
                dontStop = 0;
            }
        }


        if(printOutput(&outputParam, &arg) < 0)
        {
            printerror();
            closeProgram(&trml, &arg, EXIT_FAILURE);
        }
    }


    closeProgram(&trml, &arg, EXIT_SUCCESS);
}
