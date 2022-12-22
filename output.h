#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include "argument1.h"
#include "buffer1.h"

#define MAX_OFFSET_SPACE_WIDTH 20

int print_offset(long currentPOS, int currentBufferSize);
int print_space(int count);
int print_hex_value(buffer *buffername);
int print_char_value(struct argument *arg, buffer *buffername);
int printOutput(struct argument *arg, buffer *buffername, long currentPOS);

#endif
