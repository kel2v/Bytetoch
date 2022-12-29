#ifndef _ERROR_H_
#define _ERROR_H_

#define MAX_FUNCNAMELENGTH 64

#define EINVARGCOUNT 1000000
#define EINVALFLAGS 1000001
#define EINVCOLWIDTHMOD 1000002
#define EPOSOUTRANGE 1000003
#define ESTRNOTPUREINT 1000004

static char originFuncName[MAX_FUNCNAMELENGTH] = "";

void printerror();

#endif
