#ifndef _TOOLS_H_
#define _TOOLS_H_

#define DEFAULT_COLUMN_WIDTH 16
#define MIN_COLUMN_WIDTH 1
#define MAX_COLUMN_WIDTH 64
#define MAX_OFFSET_SPACE_WIDTH 20

typedef struct arguments arguments;
typedef unsigned char buffer;

arguments *initArguments(int argc, char *argv[]);
buffer *allocateBuffer(arguments *arg);
int choice(char **choiceStr, size_t *choiceStrBufferLength);
int loadBuffer(buffer *buf, arguments *arg);
void printOutput(buffer *buf, arguments *arg, size_t *bytes_printed);
void freeArguments(arguments *arg);
void freeBuffer(buffer *buf);

#endif
