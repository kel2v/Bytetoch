#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "../headers/headers.h"
#include ARGUMENT_H
#include TERMINAL_H

void closeProgram(struct terminal *trml, struct argument *arg, int exit_status)
{
    tcsetattr(STDIN_FILENO, TCSANOW, &(trml->termios_default));
    if(arg->srcstream != NULL) fclose(arg->srcstream);
    exit(exit_status);
}
