# bin
A simple program which displays bytewise data of a binary file, where each Byte is represented as a Hexadecimal value and its respective ASCII character (if it's Printable, else represented by a whitespace)

Usage:
------

Note: When I mention "binary_src", it means either the path of the binary source, or if it is in the current working directory, then the name of the binary source file.

1) Minimum syntax: View byte-wise data of the given binary file in both ASCII character representations and Hexadecimal values with defualt column-width (16) value. Example -  
`$ ./bin binary_src`

2) "-t" flag (optional): If added before entering filename, only the ASCII character representation is displayed; Hexadeciminal values are not displayed. Example -  
`$ ./bin -t binary_src`  
`$ ./bin -c 48 -t binary_src`  
`$ ./bin -t -c 48 binary_src`

3) "-c" flag (optional): If added before entering filename, it is mandatory to add an **valid** integer value (new column-width) in the next argument, and if **valid** integer value is added as required, then "new column-width" number of consecutive byte's data is represented in one line of output. Example -  
`$ ./bin -c 32 binary_src`  
`$ ./bin -c 32 -t binary_src`  
`$ ./bin -t -c 32 binary_src`

> **Note**: A '**valid** integer value' mentioned above means the entered string must be in decimal number system, should only contain numerical characters and it's value must be between MIN_COLWIDTH (1) and MAX_COLWIDTH (128), both inclusive.
----

> **Note**: The project is in primitive stage, hence it's not garanteed that it will work properly in every system.
