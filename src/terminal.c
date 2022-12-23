#include <string.h>
#include <termios.h>
#include <errno.h>
#include <unistd.h>

#include "../headers/headers.h"
#include TERMINAL_H
#include ERROR_H


int init_terminal(struct terminal *trml, int fd)
{
    strcpy(originFuncName, "init_terminal");

    trml->fd = fd;

    if(tcgetattr(fd, &(trml->termios_default)) == -1)
    {
        return -1;
    }
    trml->termios_runtime = trml->termios_default;

    return 0;
}

int set_canon(struct terminal *trml, int mode)
{
    strcpy(originFuncName, "set_canon");

    if(mode == OFF)
    {
        trml->termios_runtime.c_lflag &= ~ICANON;
    }
    else if(mode == ON)
    {
        trml->termios_runtime.c_lflag |= ICANON;
    }

    if(tcsetattr(trml->fd, TCSANOW, &(trml->termios_runtime)) == -1)
    {
        return -1;
    }

    return 0;
}

int set_echo(struct terminal *trml, int mode)
{
    strcpy(originFuncName, "set_echo");

    if(mode == OFF)
    {
        trml->termios_runtime.c_lflag &= ~ECHO;
    }
    else if(mode == ON)
    {
        trml->termios_runtime.c_lflag |= ECHO;
    }

    if(tcsetattr(trml->fd, TCSANOW, &(trml->termios_runtime)) == -1)
    {
        return -1;
    }

    return 0;
}

/*
int reinit_terminal(terminal *trml)
{
    strcpy(originFuncName, "reinit_terminal");

    if(tcsetattr(trml->fd, TCSANOW, &trml->termios_ptr) == -1)
    {
        return -1;
    }

    return 0;
}
*/
