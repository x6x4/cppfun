
<h1>CPU MODEL</h1>

Unlike usual fetch-decode-execute cycle, this CPU do fetch-cache-execute.

_CPU internals:_

 *  Program memory:  |cmd1|cmd2|cmd3|cmd4|...|  
                     | pc | zf |
 *  
 *  Execution units: | cmd3 | | cmd1 | | free | | cmd2 |
 *  
 *  General-purpose register block:  |reg1|reg2|reg3|reg4|...|
 *  Data memory: |||||||||||||||||||||||||


_Exec process:_

User program + Instruction set
    |
Mcode compiler 
    |
Program Memory
    |
Control unit 
    |
Execution unit
    |
Cache (exec command)
    |
Register Block
Data Memory  
    |
Program Memory