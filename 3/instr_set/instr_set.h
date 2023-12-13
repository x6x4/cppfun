/** @file instr_set/instr_set.h
 *  This file contains user-defined unary and binary operators.
 */

#pragma once
#include "../lib/internals/operands/operands.h"
#include <cstddef>


//  UNARY OPERATORS

void Oper_Increment (Operand &opd1);

class Oper_Inc : public UnaryOperator {
public:
    Oper_Inc() : UnaryOperator("inc") { oper = Oper_Increment; }
};

void Oper_Jump (Operand &opd1);

class Oper_Jmp : public UnaryOperator {
public:
    Oper_Jmp() : UnaryOperator("jmp") { oper = Oper_Jump; } 
};

void Oper_Syscall (Operand &opd1, Operand &opd2);

class Oper_SysCall : public BinaryOperator {
public:
    Oper_SysCall() : BinaryOperator("syscall") { oper = Oper_Syscall; } 
};

//  BINARY OPERATORS

void Oper_Movement (Operand &opd1, Operand &opd2);

class Oper_Mov : public BinaryOperator {
public:
    Oper_Mov() : BinaryOperator("mov") { oper = Oper_Movement;}
};

//  TERNARY OPERATORS

void Oper_FindSymbol (Operand &opd1, Operand &opd2, Operand &opd3);

class Oper_FindSym : public TernaryOperator {
public:
    Oper_FindSym() : TernaryOperator("fsym") { oper = Oper_FindSymbol; } 
};

void Oper_CompareStrings (Operand &opd1, Operand &opd2, Operand &opd3);

class Oper_CmpStr : public TernaryOperator {
public:
    Oper_CmpStr() : TernaryOperator("cmpstr") { oper = Oper_CompareStrings;}
};


extern unary_instr_set uset; 
extern binary_instr_set bset;
extern ternary_instr_set tset;

extern InstrSet iset;








