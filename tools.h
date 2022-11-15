#ifndef _TOOLS_H_
#define _TOOLS_H_

#include <stdio.h>

#define MIN_COLUMN_WIDTH 1
#define MAX_COLUMN_WIDTH 64
#define DEFAULT_COLUMN_WIDTH 16
#define MAX_OFFSET_SPACE_WIDTH 20
#define MAX_CHOICESTR_BUFFER_LENGTH 1024
#define printerror() fprintf(stderr, "error: %s - %s\n", __func__, strerror(errno))

struct arguments
{
  FILE *src;
  int column_width;
  size_t filesize;
};

void *common_malloc(size_t size);
struct arguments *initArguments(int argc, char *argv[]);
int choice(char **choiceStr, size_t *choiceStrBufferLength, long *offset);
int loadBuffer(unsigned char *buf, struct arguments *arg);
void printOutput(unsigned char *buf, struct arguments *arg, size_t *bytes_printed);
void freeArguments(struct arguments *arg);

#endif
