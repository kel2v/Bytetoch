#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdarg.h>
#include "tools.h"

void *common_malloc(size_t size)
{
  void *ptr = malloc(size);
  if(size != 0 && ptr == NULL)
  {
    printerror();
    return NULL;
  }
  return ptr;
}


int init_src(struct arguments *arg, int argc, char *argv[])
{
  int index;

  index = (argc == 2)?1:2;
  if(access(argv[index], F_OK) != 0 || (arg->src = fopen(argv[index], "rb")) == NULL)
  {
    printerror();
    return -1;
  }

  return 0;
}

int init_column_width(struct arguments *arg, int argc, char *argv[])
{
  char *endptr = NULL;

  arg->column_width = DEFAULT_COLUMN_WIDTH;
  if(argc == 3)
  {
    arg->column_width = strtol(argv[1], &endptr, 10);
    if( (*argv[1] == '\0' || *endptr != '\0') || (arg->column_width < MIN_COLUMN_WIDTH || arg->column_width > MAX_COLUMN_WIDTH) )
    {
      fprintf(stderr, "error: init_column_width - invalid column_width modifier value; using default value %d\n", DEFAULT_COLUMN_WIDTH);
      arg->column_width = DEFAULT_COLUMN_WIDTH;
      return 1;
    }
  }

  return 0;
}

void init_filesize(struct arguments *arg)
{
  fseek(arg->src, 0, SEEK_END);
  arg->filesize = ftell(arg->src);
  fseek(arg->src, 0, SEEK_SET);
}

void freeArguments(struct arguments *arg)
{
  if(arg != NULL)
  {
    if(arg->src != NULL) fclose(arg->src);
    free(arg);
  }
}

struct arguments *initArguments(int argc, char *argv[])
{
  struct arguments *arg = NULL;

  if(argc <= 1 || argc > 3)
  {
    fprintf(stderr, "initArgument - invalid number of arguments\n");
    return NULL;
  }

  if( (arg = common_malloc(sizeof(struct arguments))) == NULL )
  {
    return NULL;
  }

  if(init_src(arg, argc, argv) == -1)
  {
    free(arg);
    return NULL;
  }
  init_column_width(arg, argc, argv);
  init_filesize(arg);

  return arg;
}



int choice(char **choiceStr, size_t *choiceStrBufferLength, long *offset)
{
  ssize_t readCount;
  char *endptr = NULL;

  readCount = getline(choiceStr, choiceStrBufferLength, stdin);
  if(readCount == -1)
  {
    printerror();
    return -1;
  }
  else if(readCount == 1)
  {
    return 0;
  }
  else if(readCount == 2 && (*choiceStr[0] == 'q' || *choiceStr[0] == 'Q') )
  {
    return 1;
  }
  else
  {
    *offset = strtol(*choiceStr, &endptr, 0);
    if(*choiceStr[0] != '\0' && *endptr == '\n') return 2;
    else
    {
      return 3;
    }
  }
}

int loadBuffer(unsigned char *buf, struct arguments *arg)
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
    printerror();
    return -1;
  }

  return 0;
}

void printOutput(unsigned char *buf, struct arguments *arg, size_t *bytes_printed)
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
