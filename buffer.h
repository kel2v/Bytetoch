#ifndef _BUFFER_H_
#define _BUFFER_H_

#include "argument1.h"

#define MAX_BUFFERSIZE 128

typedef struct buffer
{
    unsigned char buf[MAX_BUFFERSIZE];
    int buffersize;
    int currentBufferSize;
} buffer;

void init_buffer(buffer *buffername, int size);
int loadBuffer(buffer *buffername, struct argument *arg);

#endif
