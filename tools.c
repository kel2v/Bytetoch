#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
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
  if(access(argv[argc-1], F_OK) != 0 || (arg->src = fopen(argv[argc-1], "rb")) == NULL)
  {
    printerror();
    return -1;
  }

  return 0;
}

int init_column_width(struct arguments *arg, int argc, char *argv[])
{
  char *endptr = NULL;
  int modifier = 0;
  size_t i;

  arg->column_width = DEFAULT_COLUMN_WIDTH;
  if(argc >= 3)
  {
    for(i=1; i<argc-1; i++)
    {
      if(strcmp(argv[i], "-c") == 0)
      {
        modifier = i+1;
        break;
      }
    }

    if(modifier != 0)
    {
      arg->column_width = strtol(argv[modifier], &endptr, 10);
      if( *argv[modifier] == '\0' || *endptr != '\0' || arg->column_width < MIN_COLUMN_WIDTH || arg->column_width > MAX_COLUMN_WIDTH )
      {
        fprintf(stderr, "error: init_column_width - invalid column_width modifier value; using default value %d\n", DEFAULT_COLUMN_WIDTH);
        arg->column_width = DEFAULT_COLUMN_WIDTH;
        return 1;
      }
    }
  }

  return 0;
}

void init_onlytext(struct arguments *arg, int argc, char *argv[])
{
  size_t i;

  arg->onlytext = 0;
  if(argc >= 3)
  {
    for(i=1; i<argc-1; i++)
    {
      if(strcmp(argv[i], "-t") == 0)
      {
        arg->onlytext = 1;
      }
    }
  }
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
  size_t i;

  if(argc <= 1 || argc > 5)
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
  init_onlytext(arg, argc, argv);
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

size_t print_offset(struct arguments *arg, size_t bytes_printed)
{
  return fprintf(stdout, "%lx-%lx", bytes_printed, bytes_printed+arg->column_width-1);
}

int print_space(int count)
{
  size_t i;
  for(i=0; i<count; i++) fprintf(stdout, " ");
  return count;
}

int print_hex_value(unsigned char *buf, struct arguments *arg)
{
  size_t i;
  for(i=0; i<arg->column_width; i++) fprintf(stdout, "%02x ", buf[i]);
  return arg->column_width;
}

int print_char_value(unsigned char *buf, struct arguments *arg)
{
  size_t i;
  if(arg->onlytext == 1)
  {
    for(i=0; i<arg->column_width; i++) fprintf(stdout, "%c", (buf[i]>=32 && buf[i]<=126)?buf[i]:' ');
  }
  else
  {
    for(i=0; i<arg->column_width; i++) fprintf(stdout, " %c ", (buf[i]>=32 && buf[i]<=126)?buf[i]:' ');
  }
  return arg->column_width;
}

void printOutput(unsigned char *buf, struct arguments *arg, size_t *bytes_printed)
{
  size_t remaining_indent, i;

  if(arg->onlytext == 1)
  {
    remaining_indent = print_offset(arg, *bytes_printed);
    remaining_indent = MAX_OFFSET_SPACE_WIDTH - remaining_indent;
    print_space(remaining_indent);
    print_char_value(buf, arg);
    fprintf(stdout, "\n");
  }
  else
  {
    remaining_indent = print_offset(arg, *bytes_printed);
    remaining_indent = MAX_OFFSET_SPACE_WIDTH - remaining_indent;
    print_space(remaining_indent);
    print_hex_value(buf, arg);
    fprintf(stdout, "\n");
    print_space(MAX_OFFSET_SPACE_WIDTH);
    print_char_value(buf, arg);
    fprintf(stdout, "\n");
  }

  *bytes_printed += arg->column_width;
}
