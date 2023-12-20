/** @file instr_set/instr_set.h
 *  This file contains user-defined unary and binary operators.
 */

#pragma once
#include "../lib/internals/operands/operands.h"
#include <cstddef>


//  UNARY OPERATORS

void Oper_Increment (my_std::Vec<std::unique_ptr<Operand>> &opds);

class Oper_Inc : public Operator {
public:
    Oper_Inc() : Operator("inc") { oper = Oper_Increment; }
};

void Oper_Jump (my_std::Vec<std::unique_ptr<Operand>> &opds);

class Oper_Jmp : public Operator {
public:
    Oper_Jmp() : Operator("jmp") { oper = Oper_Jump; } 
};


//  BINARY OPERATORS

void Oper_Movement (my_std::Vec<std::unique_ptr<Operand>> &opds);

class Oper_Mov : public Operator {
public:
    Oper_Mov() : Operator("mov") { oper = Oper_Movement;}
};

void Oper_Syscall (my_std::Vec<std::unique_ptr<Operand>> &opds);

class Oper_FindSymbol : public Operator {
public:
    Oper_FindSymbol() : Operator("syscall") { oper = Oper_Syscall; } 
};

//  TERNARY OPERATORS

void Oper_FindSymbo (my_std::Vec<std::unique_ptr<Operand>> &opds);

class Oper_FindSym : public Operator {
public:
    Oper_FindSym() : Operator("fsym") { oper = Oper_FindSymbo; } 
};

void Oper_CompareStrings (my_std::Vec<std::unique_ptr<Operand>> &opds);

class Oper_CmpStr : public Operator {
public:
    Oper_CmpStr() : Operator("cmpstr") { oper = Oper_CompareStrings;}
};


extern InstrSet iset;








