#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "tools.h"

int main(int argc, char *argv[])
{
  struct arguments *arg = NULL;
  unsigned char *buf;
  char *choiceStr = NULL;
  int choiceValue, bufferStatus, more = 1;
  long current_pos, offset;
  size_t bytes_printed = 0, choiceStrBufferLength = MAX_CHOICESTR_BUFFER_LENGTH;

  if((arg = initArguments(argc, argv)) == NULL)
  {
    return EXIT_FAILURE;
  }

  if( ((buf = malloc(arg->column_width+1)) == NULL) && ((choiceStr = malloc(choiceStrBufferLength)) == NULL) )
  {
    freeArguments(arg);
    return EXIT_FAILURE;
  }

  while(more)
  {
  	choiceValue = choice(&choiceStr, &choiceStrBufferLength, &offset);
    if(choiceValue == -1)
    {
      freeArguments(arg);
      free(choiceStr);
      return EXIT_FAILURE;
    }
    else if(choiceValue == 1)
    {
      break;
    }
    else if(choiceValue == 2)
    {
      current_pos = ftell(arg->src);
      bytes_printed = offset - offset%arg->column_width;
      if( (fseek(arg->src, offset, SEEK_SET) == -1) || (ftell(arg->src) >= arg->filesize) )
      {
        fseek(arg->src, current_pos, SEEK_SET);
        bytes_printed = current_pos;
        continue;
      }
    }
    else if(choiceValue == 3)
    {
      continue;
    }

    bufferStatus = loadBuffer(buf, arg);
    if(bufferStatus == -1)
    {
      freeArguments(arg);
      free(choiceStr);
      return EXIT_FAILURE;
    }
    else if(bufferStatus == 1)
    {
      more = 0;
    }

    printOutput(buf, arg, &bytes_printed);
  }

  freeArguments(arg);
  free(choiceStr);
  return EXIT_SUCCESS;
}
