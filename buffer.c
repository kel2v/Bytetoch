#include <stdio.h>
#include <string.h>

#include "argument1.h"
#include "buffer1.h"
#include "error1.h"

void init_buffer(buffer *buffername, int size)
{
    buffername->buffersize = size;
    buffername->currentBufferSize = buffername->buffersize;
}

int loadBuffer(buffer *buffername, struct argument *arg)
{
    strcpy(originFuncName, "loadBuffer");

    int readCount = fread(buffername->buf, 1, buffername->currentBufferSize, arg->srcstream);
    if(readCount != buffername->currentBufferSize)
    {
        if(feof(arg->srcstream) != 0)
        {
            buffername->currentBufferSize = readCount;
        }

        if(ferror(arg->srcstream) != 0)
        {
          return -1;
        }
    }

    return readCount;
}
