Q8 & QASM

This project implements custom CPU 8-bit architecture and its assembler.

Some limitations:
    - identifiers consist of up to 8 latin letters 

Assembler features:
    - detailed compilation errors
    - comments


Command syntax:  

            [label] oper opd1       [opd2]
 
un_instr:   [label] oper reg/label 

bin_instr:  [label] oper reg/label  reg/label/imm op

result --> opd1

Command list:

Unary: 
    jmp
    je
    jne

    inc
    neg

Binary:
    cmp
    mov



