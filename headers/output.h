#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include "argument.h"

#define MAX_BUFFERSIZE 32768+1  // 32 KiB + 1 Null terminator
#define MAX_OFFSET_SPACE_WIDTH 20

struct output
{
	int initialColWidth;
	int currentColWidth;
	int pointer;

	struct
	{
	    unsigned char buf[MAX_BUFFERSIZE];
	    int buffersize;
	    long bufferOffset;
	} buffer;
};

void init_Output(struct output *outputParam, struct argument *arg);
int isShortage(struct output *outputParam);
int loadOutputBuffer(struct output *outputParam, struct argument *arg);
void resetOutputBuffer(struct output *outputParam);
void reinit_outputColWidth(struct output *outputParam);

int printOutput(struct output *outputParam, struct argument *arg);
int print_offset(struct output *outputParam);
int print_space(int count);
int print_hex_value(struct output *outputParam);
int print_char_value(struct argument *arg, struct output *outputParam);

#endif
