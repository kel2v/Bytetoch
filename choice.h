#ifndef _CHOICE_H_
#define _CHOICE_H_

#include "terminal.h"
#include "argument.h"

#define MAX_INPUTBUFSIZE 64

int processChoice(struct terminal *trml, struct argument *arg, char choice);
long stringToNum(char *string);
int chStrmPosPtr(struct argument *arg, long nextPOS);

#endif
