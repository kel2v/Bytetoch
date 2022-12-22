#include <stdio.h>
#include <string.h>

#include "argument1.h"
#include "buffer1.h"
#include "output1.h"
#include "error1.h"

int printOutput(struct argument *arg, buffer *buffername, long currentPOS)
{
    strcpy(originFuncName, "printOutput");

    int remaining_indent, numOfCharPrinted;

    if(arg->onlytext == 1)
    {
        remaining_indent = print_offset(currentPOS, buffername->currentBufferSize);
        if(remaining_indent < 0)
        {
            return -1;
        }
        numOfCharPrinted = remaining_indent;
        remaining_indent = MAX_OFFSET_SPACE_WIDTH - remaining_indent;

        int a, b, c;
        if((a = print_space(remaining_indent)) < 0 || (b = print_char_value(arg, buffername)) < 0 || (c = fprintf(stdout, "\n")) < 0)
        {
            return -1;
        }

        numOfCharPrinted += a+b+c;
    }
    else
    {
        remaining_indent = print_offset(currentPOS, buffername->currentBufferSize);
        if(remaining_indent < 0)
        {
            return -1;
        }
        numOfCharPrinted = remaining_indent;
        remaining_indent = MAX_OFFSET_SPACE_WIDTH - remaining_indent;

        int a, b, c, d, e, f;
        if( (a = print_space(remaining_indent)) < 0 || (b = print_hex_value(buffername)) < 0 || (c = fprintf(stdout, "\n")) < 0 || (d = print_space(MAX_OFFSET_SPACE_WIDTH)) < 0 || (e = print_char_value(arg, buffername)) < 0 || (f = fprintf(stdout, "\n\n")) < 0 )
        {
            return -1;
        }

        numOfCharPrinted += a+b+c+d+e+f;
    }

    return numOfCharPrinted;
}

int print_offset(long currentPOS, int currentBufferSize)
{
    return fprintf(stdout, "%lx-%lx", currentPOS, currentPOS+currentBufferSize-1);
}

int print_space(int count)
{
    for(int i=0; i<count; i++)
    {
        if( (fprintf(stdout, " ")) < 0 )
        {
            return -1;
        }
    }

    return count;
}

int print_hex_value(buffer *buffername)
{
    int numOfCharPrinted = 0;
    for(int temp, i=0; i<buffername->currentBufferSize; i++)
    {
        if( (temp = fprintf(stdout, "%02x ", buffername->buf[i])) < 0)
        {
            return -1;
        }
        numOfCharPrinted += temp;
    }

    return numOfCharPrinted;
}

int print_char_value(struct argument *arg, buffer *buffername)
{
    int numOfCharPrinted = 0;
    if(arg->onlytext == 1)
    {
        for(int temp, i=0; i<buffername->currentBufferSize; i++)
        {
            if( (temp = fprintf(stdout, "%c", (buffername->buf[i]>=32 && buffername->buf[i]<=126)?buffername->buf[i]:' ')) < 0)
            {
                return -1;
            }

            numOfCharPrinted += temp;
        }
    }
    else
    {
        for(long temp, i=0; i<buffername->currentBufferSize; i++)
        {
            if( (temp = fprintf(stdout, " %c ", (buffername->buf[i]>=32 && buffername->buf[i]<=126)?buffername->buf[i]:' ')) < 0)
            {
                return -1;
            }

            numOfCharPrinted += temp;
        }
    }

    return numOfCharPrinted;
}
