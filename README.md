# bin
A simple program which displays bytewise data of a binary file, where each Byte is represented as a Hexadecimal value and its respective ASCII character (if it's Printable, else represented by a whitespace)

Note: The project is in primitive stage, hence it's not garanteed that it will work properly in any given system.

---

Source files
------------
The source code is splited into two files: main.h and main.c.  
**main.h**: contains all the struct definitions and user-defined functions used in the 'main' function of the program.  
**main.c**: contains the 'main' function code

---

How it works?
-------------

When the program is executed by calling it from a shell, optionally with column-width modifier argument, the process runs as given below -

1. Validate the arguments: The program checks for the existence of the given binary file and also checks if the column-width modifier (if provided) argument is a valid integer. A column-width modifier argument is a valid integer if all these conditions are satisfied -
    1. If entered in decimal format:
      * Should only contain numeric characters 0-9, inclusive.
      * Should not begin with 0
    2. If entered in Octal format:
      * Should only contain numeric characters 0-7, inclusive.
      * should begin with 0.
    3. If entered in Hexadecimal format:
      *
2. Initialize the arguments to a internal object: If the given file exists and the program has access to read it, the file is **open**ed and if the column-width modifier argument (if provided) is valid, its value is assigned to the **column-width** variable.
3. A buffer of size equal to **column-width** bytes is created to store the **column-width** number of consecutive bytes' contained in the binary file.
4. Loop:
    1. Takes in a line of user's choice input
      * If choice input contains only a newline character, next line of output is written in stdout
      * If choice input contains only two characters - 'q'/'Q' and a newline character, then the loop **break**s
      * If choice input contains more than 'q'/'Q' and a newline character, it is discarded and **continue**d to next iteration.
    2. Reads next **column-width** number of bytes' from the current cursor in the binary file and stores it in the **buffer**.
      * If EOF is set ON, then a `last` flag is set 1 to **break** the loop at the end of the current iteration.
      * If an error occurs while `fread`ing, the process is terminated with EXIT_FAILURE.
    3. Prints the output; the offset, the indenting, the hexadecimal and ASCII representation of the **column-witdh** number of bytes.


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
