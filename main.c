#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>

typedef struct arguments
{
  FILE *source;
  size_t column;
} arguments;

bool isOnlyNumeral(char *str)
{
  size_t length = strlen(str), i;

  for(i=0; i<length ; i++)
  {
    if(!(str[i] >= 48 && str[i]<=57)) return false;
  }

  return true;
}

arguments *validateArg(int argc, char *argv[], size_t column)
{
  arguments *arg;
  arg = malloc(sizeof(arguments));
  if(arg == NULL)
  {
    fprintf(stderr, "fatal error : arg -> %s\n", strerror(errno));
    return NULL;
  }

  if(argc == 1)
  {
    fprintf(stderr, "fatal error : too few arguments\n");
    free(arg);
    return NULL;
  }
  else if(argc == 2)
  {
    if(access(argv[1], F_OK) == -1)
    {
      fprintf(stderr, "fatal error : %s -> %s\n", argv[1], strerror(errno));
      free(arg);
      return NULL;
    }
    else
    {
      arg->source = fopen(argv[1], "rb");
      if(arg->source == NULL)
      {
        fprintf(stderr, "fatal error : %s -> %s\n", argv[1], strerror(errno));
        free(arg);
        return NULL;
      }
      arg->column = column;
    }
  }
  else if(argc == 3)
  {
    if(access(argv[2], F_OK) == -1)
    {
      fprintf(stderr, "fatal error : %s -> %s\n", argv[1], strerror(errno));
      free(arg);
      return NULL;
    }
    else
    {
      arg->source = fopen(argv[2], "rb");
      if(arg->source == NULL)
      {
        fprintf(stderr, "fatal error : %s -> %s\n", argv[1], strerror(errno));
        free(arg);
        return NULL;
      }

      if(!isOnlyNumeral(argv[1]))
      {
        fprintf(stderr, "error : argument 1 is not an integer; using default value\n");
        arg->column = column;
      }
      else arg->column = (size_t) atoi(argv[1]);
    }
  }
  else
  {
    fprintf(stderr, "fatal error : too many arguments\n");
    free(arg);
    return NULL;
  }

  return arg;
}

int main(int argc, char *argv[])
{
  bool next, last, exit;
  char *line = NULL;
  unsigned char *buffer = NULL;
  size_t i, buffercount, linlen, linesize;
  arguments *arg = NULL;


  line = malloc(1);
  if(line == NULL)
  {
    fprintf(stderr, "fatal error : line -> %s", strerror(errno));
    return EXIT_FAILURE;
  }

  arg = validateArg(argc, argv, 16);
  if(arg == NULL)
  {
    free(line);
    return EXIT_FAILURE;
  }

  printf("column %lu\n", arg->column);

  buffer = malloc(arg->column+1);
  if(buffer == NULL)
  {
    fprintf(stderr, "fatal error : buffer -> %s\n", strerror(errno));
    free(line);
    fclose(arg->source);
    free(arg);
    return EXIT_FAILURE;
  }

  last = false;
  linlen = 0;
  while(1)
  {
    next = false;

  	linesize = getline(&line, &linlen, stdin);
    if(linesize == -1)
    {
      fprintf(stderr, "fatal error : linesize -> %s\n", strerror(errno));
      free(line);
      fclose(arg->source);
      free(arg);
      free(buffer);
      return EXIT_FAILURE;
    }

    if(linesize > 1)
    {
      for(i=0; i<linesize-1; i++)
      {
        if(line[i] == 'q' || line[i] == 'Q')
        {
          exit = true;
          break;
        }
      }

      next = true;
    }

    if(exit) break;
    else if(next) continue;

    buffercount = fread(buffer, sizeof(char), arg->column, arg->source);
    if(buffercount == 0)
    {
      if(feof(stdin) != 0)
      {
        arg->column = buffercount;
        last = true;
      }
      else
      {
        fprintf(stderr, "fatal error : fread -> %s\n", strerror(errno));
        free(line);
        fclose(arg->source);
        free(arg);
        free(buffer);
        return EXIT_FAILURE;
      }
    }

    for(i=0; i<arg->column; i++) fprintf(stdout, "%02x ", buffer[i]);
    fprintf(stdout, "\n");

    for(i=0; i<arg->column; i++) fprintf(stdout, " %c ", (buffer[i]>=32 && buffer[i]<=126)?buffer[i]:'.');
    fprintf(stdout, "\n");

    if(last) break;
  }

  free(line);
  fclose(arg->source);
  free(arg);
  free(buffer);

  return EXIT_SUCCESS;
}
