#include <stdio.h>
#include <string.h>

#include "../headers/headers.h"
#include ARGUMENT_H
#include OUTPUT_H
#include ERROR_H

void init_Output(struct output *outputParam, struct argument *arg)
{
    outputParam->initialColWidth = arg->column_width;
    outputParam->currentColWidth = arg->column_width;
    outputParam->pointer = 0;

    outputParam->buffer.buf[0] = '\0';
    outputParam->buffer.buffersize = 0;
    outputParam->buffer.bufferOffset = 0;
}


int isShortage(struct output *outputParam)
{
    if(outputParam->buffer.buffersize < outputParam->initialColWidth) return 1;
    else return 0;
}


int loadOutputBuffer(struct output *outputParam, struct argument *arg)
{
    long bufferOffset = ftell(arg->srcstream)-outputParam->buffer.buffersize;
    if(fseek(arg->srcstream, bufferOffset, SEEK_SET) == -1)
    {
        strcpy(originFuncName, "loadOutputBuffer");
        return -1;
    }

    int readCount = fread(outputParam->buffer.buf, 1, MAX_BUFFERSIZE-1, arg->srcstream);
    
    if(ferror(arg->srcstream) != 0)
    {
        strcpy(originFuncName, "loadOutputBuffer");
        return -1;
    }

    outputParam->pointer = 0;
    outputParam->buffer.buffersize = readCount;
    outputParam->buffer.bufferOffset = bufferOffset;

    return readCount;
}

void resetOutputBuffer(struct output *outputParam)
{
    outputParam->buffer.buffersize = 0;
}


void reinit_outputColWidth(struct output *outputParam)
{
    outputParam->currentColWidth = outputParam->buffer.buffersize;
}


int printOutput(struct output *outputParam, struct argument *arg)
{
    int remaining_indent, numOfCharPrinted;

    if(arg->onlytext == 1)
    {
        remaining_indent = print_offset(outputParam);
        if(remaining_indent < 0)
        {
            strcpy(originFuncName, "printOutput");
            return -1;
        }
        numOfCharPrinted = remaining_indent;
        remaining_indent = MAX_OFFSET_SPACE_WIDTH - remaining_indent;

        int a, b, c;
        if((a = print_space(remaining_indent)) < 0 || (b = print_char_value(arg, outputParam)) < 0 || (c = fprintf(stdout, "\n")) < 0)
        {
            strcpy(originFuncName, "printOutput");
            return -1;
        }

        numOfCharPrinted += a+b+c;
    }
    else
    {
        remaining_indent = print_offset(outputParam);
        if(remaining_indent < 0)
        {
            strcpy(originFuncName, "printOutput");
            return -1;
        }
        numOfCharPrinted = remaining_indent;
        remaining_indent = MAX_OFFSET_SPACE_WIDTH - remaining_indent;

        int a, b, c, d, e, f;
        if( (a = print_space(remaining_indent)) < 0 || (b = print_hex_value(outputParam)) < 0 || (c = fprintf(stdout, "\n")) < 0 || (d = print_space(MAX_OFFSET_SPACE_WIDTH)) < 0 || (e = print_char_value(arg, outputParam)) < 0 || (f = fprintf(stdout, "\n\n")) < 0 )
        {
            strcpy(originFuncName, "printOutput");
            return -1;
        }

        numOfCharPrinted += a+b+c+d+e+f;
    }

    outputParam->buffer.buffersize -= outputParam->currentColWidth;
    outputParam->pointer += outputParam->currentColWidth;

    return numOfCharPrinted;
}

int print_offset(struct output *outputParam)
{
    return fprintf(stdout, "%lx-%lx", outputParam->buffer.bufferOffset+outputParam->pointer, outputParam->buffer.bufferOffset+outputParam->pointer+outputParam->currentColWidth-1);
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

int print_hex_value(struct output *outputParam)
{
    int numOfCharPrinted = 0;
    for(int temp, i=outputParam->pointer; i<outputParam->pointer+outputParam->currentColWidth; i++)
    {
        if( (temp = fprintf(stdout, "%02x ", outputParam->buffer.buf[i])) < 0)
        {
            return -1;
        }
        numOfCharPrinted += temp;
    }

    return numOfCharPrinted;
}

int print_char_value(struct argument *arg, struct output *outputParam)
{
    int numOfCharPrinted = 0;
    if(arg->onlytext == 1)
    {
        for(int temp, i=outputParam->pointer; i<outputParam->pointer+outputParam->currentColWidth; i++)
        {
            if( (temp = fprintf(stdout, "%c", (outputParam->buffer.buf[i]>=32 && outputParam->buffer.buf[i]<=126)?outputParam->buffer.buf[i]:'.')) < 0)
            {
                return -1;
            }

            numOfCharPrinted += temp;
        }
    }
    else
    {
        for(int temp, i=outputParam->pointer; i<outputParam->pointer+outputParam->currentColWidth; i++)
        {
            if( (temp = fprintf(stdout, " %c ", (outputParam->buffer.buf[i]>=32 && outputParam->buffer.buf[i]<=126)?outputParam->buffer.buf[i]:'.')) < 0)
            {
                return -1;
            }

            numOfCharPrinted += temp;
        }
    }

    return numOfCharPrinted;
}
