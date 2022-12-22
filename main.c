#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "terminal1.h"
#include "argument1.h"
#include "buffer1.h"
#include "choice1.h"
#include "output1.h"
#include "error1.h"
#include "close1.h"


int main(int argc, char *argv[])
{
    struct terminal trml;
    if(init_terminal(&trml, STDIN_FILENO) == -1)
    {
        printerror();
        return EXIT_FAILURE;
    }


    struct argument arg;
    int initArgumentStatus = init_Argument(&arg, argc, argv);
    if(initArgumentStatus == 1)
    {
        printerror();
    }
    else if(initArgumentStatus == -1 || initArgumentStatus == -2)
    {
        printerror();
        closeProgram(&trml, &arg, EXIT_FAILURE);
    }


    buffer outputbuffer;
    init_buffer(&outputbuffer, arg.column_width);


    if(set_canon(&trml, OFF) == -1 || set_echo(&trml, OFF) == -1)
    {
        printerror();
        closeProgram(&trml, &arg, EXIT_FAILURE);
    }


    int dontStop = 1;
    while(dontStop)
    {
        char choice = fgetc(stdin);


        int processChoiceStatus = processChoice(&trml, &arg, choice);
        if(processChoiceStatus == 2)
        {
            dontStop = 0;
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
        else if(processChoiceStatus == -2)
        {
            printerror();
            continue;
        }

        long initialOffset = ftell(arg.srcstream);
        int loadBufferStatus = loadBuffer(&outputbuffer, &arg);
        if(loadBufferStatus == -1)
        {
            printerror();
            closeProgram(&trml, &arg, EXIT_FAILURE);
        }
        else if(loadBufferStatus != outputbuffer.buffersize)
        {
            dontStop = 0;
        }


        if(printOutput(&arg, &outputbuffer, initialOffset) < 0)
        {
            printerror();
            closeProgram(&trml, &arg, EXIT_FAILURE);
        }
    }


    closeProgram(&trml, &arg, EXIT_SUCCESS);
}
