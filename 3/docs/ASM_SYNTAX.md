
This project implements custom CPU architecture and its assembler.

Some limitations:
    - labels consist of up to 8 latin letters 

Assembler features:
    - detailed compilation errors
    - comments


_Command syntax:_  
 
un_instr:   [label] oper opd1 (reg/label)
bin_instr:  [label] oper opd1 (reg/label) opd2 (reg/label/imm op)

result --> opd1


_Command list:_

*Unary:*
    inc (+)

    jmp
    je
    jne

    neg

*Binary:*
    cmp
    mov (+)



