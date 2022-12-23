#ifndef _ARGUMENT_H_
#define _ARGUMENT_H_

#include <stdio.h>

#define DEFAULT_COLWIDTH 16
#define MIN_COLWIDTH 1
#define MAX_COLWIDTH 128

struct argument
{
    FILE *srcstream;
    size_t filesize;
    int onlytext;
    int column_width;
};

int init_Argument(struct argument *arg, int argc, char *argv[]);

int init_src(struct argument *arg, int argc, char *argv[]);
int init_columnWidth(struct argument *arg, int argc, char *argv[]);
int init_onlyText(struct argument *arg, int argc, char* argv[]);
int init_fileSize(struct argument *arg);

#endif
