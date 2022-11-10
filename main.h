#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdarg.h>
#include <limits.h>

typedef struct arguments
{
  FILE *source;
  size_t column;
} arguments;


arguments *validateArguments(int argc, char *argv[], size_t column);
int initializeArg(arguments *arg, char *argument1, size_t column);
void freePtrs(int count, ...);


arguments *validateArguments(int argc, char *argv[], size_t column)
{
  long argval;
  char *endptr = NULL;
  arguments *arg;

  arg = malloc(sizeof(arguments));
  if(arg == NULL)
  {
    fprintf(stderr, "error : arg -> %s\n", strerror(errno));
    return NULL;
  }

  if(argc == 1)
  {
    fprintf(stderr, "error : too few arguments\n");
    freePtrs(1, (void *)arg);
    return NULL;
  }
  else if(argc == 2)
  {
    if(initializeArg(arg, argv[1], column) != 0) return NULL;
  }
  else if(argc == 3)
  {
    if(initializeArg(arg, argv[2], column) != 0) return NULL;

    argval = strtol(argv[1], &endptr, 0);
    if(*argv[1] == '\0' || *endptr != '\0') fprintf(stderr, "error : argument 1 -> invalid argument; using default value 16\n");
    else if(argval < 1 || argval > 64) fprintf(stderr, "error : argument 1 -> unsupported value; using default value 16\n");
    else if(argval == LONG_MIN || argval == LONG_MAX) fprintf(stderr, "error : argument 1 -> %s; using default value 16\n", strerror(errno));
    else arg->column = argval;
  }
  else
  {
    fprintf(stderr, "error : too many arguments\n");
    freePtrs(1, (void *)arg);
    return NULL;
  }

  return arg;
}


int initializeArg(arguments *arg, char *column_update_argument, size_t column)
{
  if(access(column_update_argument, F_OK) == -1)
  {
    fprintf(stderr, "error : %s -> %s\n", column_update_argument, strerror(errno));
    freePtrs(1, (void *)arg);
    return -1;
  }

  arg->source = fopen(column_update_argument, "rb");
  if(arg->source == NULL)
  {
    fprintf(stderr, "error : %s -> %s\n", column_update_argument, strerror(errno));
    freePtrs(1, (void *)arg);
    return -2;
  }
  arg->column = column;

  return 0;
}


void freePtrs(int count, ...)
{
  va_list ptrs;

  va_start(ptrs, count);
  while(count--)
  {
    free(va_arg(ptrs, void *));
  }
  va_end(ptrs);
}
