#ifndef _ERROR_H_
#define _ERROR_H_

#define MAX_FUNCNAMELENGTH 64

#define EINVARGCOUNT 1000000
#define EINVCOLWIDTHMOD 1000001
#define EPOSOUTRANGE 1000002
#define ESTRNOTPUREINT 1000003

static char originFuncName[MAX_FUNCNAMELENGTH] = "";

void printerror();

#endif
