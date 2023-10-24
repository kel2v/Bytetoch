# Bytetoch
A simple linux's terminal tool which displays bytewise data of a binary file, where each Byte can be viewed as its corresponding hexadecimal value and an ASCII character (if it's not printable, it is represented by a whitespace)

Usage:
------

Consider we have a binary file named "binary_src" (either use absolute path of it, or relative path w.r.t to the current working directory, i.e., the directory in which this tool is located). Now we can view its bytewise data in any of these following ways using appropriate options:

1) Default view: If no options were given, by default, Bytetoch displays byte-wise data of the "binary_src" in both ASCII character representation and Hexadecimal value with defualt column-width value of 16.

     Syntax:  
    `$ ./bin binary_src`

2) `-t` option (optional): If you want to view the bytewise data of the "binary_src" in only in its corresnding ASCII character representation (without its corresponding hexadecimal value), then you can use `-t` option.  

    Syntax:    
    `$ ./bin -t binary_src`  

3) `-c` option (optional): If you want to change the number of Bytes that is displayed in each row, you can use the `-c` option. Note that if this option is used, then the new column-width value should be a **valid integer value** and should be entered as the immediate next argument to this option.

    Syntax:    
    `$ ./bin -c 32 binary_src`  

4) both `-t` and `-c` (optional): If you need to view byte-wise data of the given binary file in **only** in its corresponding ASCII character, **and** with the number of columns of your choice, you can achieve this using both these options in one of the two formats given below:

    Syntax:  
    `$ ./bin -t -c 32 binary_src`  
    `$ ./bin -c 32 -t binary_src`

  
> **Note**: A '**valid** integer value' mentioned above means the entered string must be in decimal number system, should only contain numerical characters and it's value must be between MIN_COLWIDTH (1) and MAX_COLWIDTH (128), both inclusive.

View Contol
----
Once you start viewing your binary file's bytewise data in any of the ways shown above, you could control what you view next in the following ways:  
1) **Next row**: Bytetoch shows bytewise data of a binary file one line at a time, where in each line, a certain number of the bytes is displayed, as determined by the column-width value, which is, by default, is 16 but you can change it to any number in range from 1 to 128 (inclusively) using `-c` option. So, in order to view the next line, you can either click the **spacebar** or the **enter** key.
2) **Jump to**: You can jump to read bytewise data that's located at any place if you know its bytewise position. In order to jump, first you need to press **esc** (escape key), then either a **valid integer valu** either in decimal, binary, octal or hexadecimal form and finally enter the **enter** key. Then onwards, Bytetoch starts displaying with the line in which your desired byte is located being displayed as the first line (column). If entered value is not a valid integer, Bytetoch ignores the value and continues to diplay from where it was.
3) **Exit**: You can stop viewing by first pressing **esc** (escape key), then enter the 'q' or 'Q' character and finally pressing the **enter** key.
