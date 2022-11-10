#include "main.h"

int main(int argc, char *argv[])
{
  char *choiceStr = NULL;
  unsigned char *buffer = NULL;
  int next, last, exit;
  size_t i, buffercount, choiceCount, choiceStrBufferLength, bytes_printed, offset_indent_rem;
  arguments *arg = NULL;

  arg = validateArguments(argc, argv, 16);
  if(arg == NULL) return EXIT_FAILURE;

  next = 0;
  last = 0;
  exit = 0;
  buffer = malloc(arg->column+1);
  if(buffer == NULL)
  {
    fprintf(stderr, "error : buffer -> %s\n", strerror(errno));
    fclose(arg->source);
    freePtrs(1, (void *)arg);
    return EXIT_FAILURE;
  }
  choiceStrBufferLength = 0;
  bytes_printed = 0;

  while(1)
  {
    next = 0;

  	choiceCount = getline(&choiceStr, &choiceStrBufferLength, stdin);
    if(choiceCount == -1)
    {
      fprintf(stderr, "error : choiceCount -> %s\n", strerror(errno));
      fclose(arg->source);
      freePtrs(3, (void *)choiceStr, (void *)arg, (void *)buffer);
      return EXIT_FAILURE;
    }
    else if(choiceCount == 2)
    {
      if(choiceStr[0] == 'q' || choiceStr[0] == 'Q') exit = 1;
    }
    else if(choiceCount > 2) next = 1;

    if(exit) break;
    if(next) continue;

    buffercount = fread(buffer, 1, arg->column, arg->source);
    if(feof(arg->source) != 0)
    {
      arg->column = buffercount;
      last = 1;
    }
    if(ferror(arg->source) != 0)
    {
      fprintf(stderr, "error : fread -> %s\n", strerror(errno));
      fclose(arg->source);
      freePtrs(3, (void *)choiceStr, (void *)arg, (void *)buffer);
      return EXIT_FAILURE;
    }

    offset_indent_rem = fprintf(stdout, "%lx-%lx", bytes_printed, bytes_printed+arg->column-1);
    offset_indent_rem = 20 - offset_indent_rem;
    for(i=0; i<offset_indent_rem; i++) fprintf(stdout, " ");
    for(i=0; i<arg->column; i++) fprintf(stdout, "%02x ", buffer[i]);
    fprintf(stdout, "\n");
    fprintf(stdout, "                    ");
    for(i=0; i<arg->column; i++) fprintf(stdout, " %c ", (buffer[i]>=32 && buffer[i]<=126)?buffer[i]:' ');
    fprintf(stdout, "\n");

    bytes_printed += arg->column;
    if(last) break;
  }

  fclose(arg->source);
  freePtrs(3, (void *)choiceStr, (void *)arg, (void *)buffer);

  return EXIT_SUCCESS;
}
