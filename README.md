# Bytetoch
A simple linux's terminal tool which displays bytewise data of a binary file, where each Byte can be viewed as its corresponding hexadecimal value and an ASCII character (if it's not printable, it is represented by a whitespace)

Usage:
------

Consider we have a binary file named "binary_src" (either use absolute path of it, or relative path w.r.t to the current working directory, i.e., the directory in which this tool is located). Now we can view its bytewise data in any of these following ways using appropriate options:

1) Default view: If no options were given, by default, Bytetoch displays byte-wise data of the "binary_src" in both ASCII character representation and Hexadecimal value with defualt column-width value of 16.

Syntax:  
`$ ./bin binary_src`

2) "-t" option (optional): If you want to view the bytewise data of the "binary_src" in only in its corresnding ASCII character representation (without its corresponding hexadecimal value), then you can use `-t` option.  

Syntax:    
`$ ./bin -t binary_src`  

3) "-c" option (optional): If you want to change the number of Bytes that is displayed in each row, you can use the `-c` option. Note that if this option is used, then the new column-width value should be a valid integer value and should be entered as the immediate next argument to this option.

Syntax:    
`$ ./bin -c 32 binary_src`  

4) both "-t" and "-c" (optional): If you need to view byte-wise data of the given binary file in **only** in its corresponding ASCII character, **and** with the number of columns of your choice, you can achieve this using both these options in one of the two formats given below:  
`$ ./bin -t -c 32 binary_src`  
`$ ./bin -c 32 -t binary_src`

> **Note**: A '**valid** integer value' mentioned above means the entered string must be in decimal number system, should only contain numerical characters and it's value must be between MIN_COLWIDTH (1) and MAX_COLWIDTH (128), both inclusive.
----

> **Note**: The project is in primitive stage, hence it's not garanteed that it will work properly in every system.
