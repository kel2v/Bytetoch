#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "tools.h"

int main(int argc, char *argv[])
{
  buffer *buf = NULL;
  arguments *arg = NULL;
  char *choiceStr = NULL;
  int choiceValue, bufferStatus, more;
  size_t choiceStrBufferLength, bytes_printed;

  /*
   * prototype:
   * arguments *initArguments(int argc, char *argv[]);
   *
   * Description:
   * Allocates and initializes an object of type 'arguments' and returns its address.
   * The memory allocated should be freed by the function 'freeArguments'
   *
   * Return value:
   * On success, returns the address of the object.
   * On failure, returns NULL.
   *
   * Error:
   * If error occurs, prints the error message in STDERR stream. Occurence of error doesn't necessarily causes function to return NULL.
   */
  if((arg = initArguments(argc, argv)) == NULL)
  {
    return EXIT_FAILURE;
  }


  /*
   * Prototype:
   * buffer *allocateBuffer(arguments *arg);
   *
   * Description:
   * Allocates an object of type 'buffer' and returns its address.
   * The memory allocated should be freed by the function 'freeBuffer'
   *
   * Return value:
   * On success, returns the address of the object.
   * On failure, returns NULL and prints error message in STDERR.
   *
   * Error:
   * If allocation fails, prints error message in STDERR and return NULL.
   */
  if((buf = allocateBuffer(arg)) == NULL)
  {
    freeArguments(arg);
    return EXIT_FAILURE;
  }

  choiceStrBufferLength = 1024;
  choiceStr = malloc(choiceStrBufferLength);
  if(choiceStr == NULL)
  {
    freeArguments(arg);
    return EXIT_FAILURE;
  }

  more = 1;
  bytes_printed = 0;

  while(more)
  {
    /*
     * Prototype:
     * int choice(char **choiceStr, size_t *choiceStrBufferLength);
     *
     * Description:
     * Processes user's choice-input string and returns appropriate choice number.
     *
     * Return value:
     * if error occurs, returns -1.
     * else if strcmp(choiceStr, "\n") == 0
     * return 1 if strcmp(choiceStr, "q\n") == 0 or strcmp(choiceStr, "Q\n") == 0
     */
  	choiceValue = choice(&choiceStr, &choiceStrBufferLength);
    if(choiceValue == -1)
    {
      fprintf(stderr, "error : getline -> %s\n", strerror(errno));
      freeArguments(arg);
      freeBuffer(buf);
      free(choiceStr);
      return EXIT_FAILURE;
    }
    else if(choiceValue == 1)
    {
      break;
    }
    else if(choiceValue == 2)
    {
      continue;
    }

    bufferStatus = loadBuffer(buf, arg);
    if(bufferStatus == -1)
    {
      fprintf(stderr, "error: loadBuffer - %s\n", strerror(errno));
      freeArguments(arg);
      freeBuffer(buf);
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
  freeBuffer(buf);
  free(choiceStr);



  return EXIT_SUCCESS;
}
