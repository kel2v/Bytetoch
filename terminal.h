#ifndef _TERMINAL_H_
#define _TERMINAL_H_

#include <termios.h>

#define OFF 0
#define ON 1

struct terminal
{
    int fd;
    struct termios termios_default;
    struct termios termios_runtime;
};


int init_terminal(struct terminal *trml, int fd);
int set_canon(struct terminal *trml, int mode);
int set_echo(struct terminal *trml, int mode);

#endif
