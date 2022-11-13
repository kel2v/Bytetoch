#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdarg.h>
#include "tools.h"

struct arguments
{
  FILE *src;
  int column_width;
};


FILE *init_src(int argc, char *argv[])
{
  FILE *src = NULL;
  int index;

  index = (argc == 2)?1:2;
  if(access(argv[index], F_OK) != 0 || (src = fopen(argv[index], "rb")) == NULL) return NULL;
  return src;
}

size_t init_column_width(int argc, char *argv[])
{
  long column_width;
  char *endptr = NULL;

  column_width = DEFAULT_COLUMN_WIDTH;
  if(argc == 3)
  {
    column_width = strtol(argv[1], &endptr, 10);
    if( (*argv[1] == '\0' || *endptr != '\0') || (column_width < MIN_COLUMN_WIDTH || column_width > MAX_COLUMN_WIDTH) )
    {
      fprintf(stderr, "error: column_width modifier - invalid input; Using default value: %hu\n", DEFAULT_COLUMN_WIDTH);
      column_width = DEFAULT_COLUMN_WIDTH;
    }
  }
  return column_width;
}


struct arguments *initArguments(int argc, char *argv[])
{
  struct arguments *arg = NULL;

  if(argc <= 1 || argc > 3)
  {
    fprintf(stderr, "error: arguments - invalid number of arguments\n");
    return NULL;
  }

  arg = malloc(sizeof(struct arguments));
  if(arg == NULL)
  {
    fprintf(stderr, "error: malloc - %s\n", strerror(errno));
    return NULL;
  }

  if((arg->src = init_src(argc, argv)) == NULL)
  {
    fprintf(stderr, "error: binary source - %s\n", strerror(errno));
    return NULL;
  }
  arg->column_width = init_column_width(argc, argv);

  return arg;
}


unsigned char *allocateBuffer(struct arguments *arg)
{
  unsigned char *buffer;
  buffer = malloc(arg->column_width+1);
  if(buffer == NULL)
  {
    fprintf(stderr, "error: allocateBuffer -> %s\n", strerror(errno));
    fclose(arg->src);
    return NULL;
  }
  return buffer;
}

int choice(char **choiceStr, size_t *choiceStrBufferLength)
{
  ssize_t readCount;
  readCount = getline(choiceStr, choiceStrBufferLength, stdin);

  if(readCount == -1) return -1;
  else if(readCount == 1) return 0;
  else if(readCount == 2)
  {
    if(*choiceStr[0] == 'q' || *choiceStr[0] == 'Q') return 1;
  }
  else return 2;
}

int loadBuffer(buffer *buf, arguments *arg)
{
  size_t buffercount;

  buffercount = fread(buf, 1, arg->column_width, arg->src);
  if(feof(arg->src) != 0)
  {
    arg->column_width = buffercount;
    return 1;
  }
  if(ferror(arg->src) != 0)
  {
    return -1;
  }

  return 0;
}

void printOutput(buffer *buf, arguments *arg, size_t *bytes_printed)
{
  size_t remaining_indent, i;

  remaining_indent = fprintf(stdout, "%lx-%lx", *bytes_printed, *bytes_printed+arg->column_width-1);
  remaining_indent = MAX_OFFSET_SPACE_WIDTH - remaining_indent;
  for(i=0; i<remaining_indent; i++) fprintf(stdout, " ");

  for(i=0; i<arg->column_width; i++) fprintf(stdout, "%02x ", buf[i]);
  fprintf(stdout, "\n");

  fprintf(stdout, "                    ");

  for(i=0; i<arg->column_width; i++) fprintf(stdout, " %c ", (buf[i]>=32 && buf[i]<=126)?buf[i]:' ');
  fprintf(stdout, "\n");

  *bytes_printed += arg->column_width;
}

void freeArguments(arguments *arg)
{
  if(arg != NULL)
  {
    if(arg->src != NULL) fclose(arg->src);
    free(arg);
  }
}

void freeBuffer(buffer *buf)
{
  free(buf);
}
