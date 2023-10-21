/** @file instr_set/instr_set.h
 *  This file contains user-defined unary and binary operators.
 */

#pragma once
#include "../lib/cpu/cpu.h"
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


//  BINARY OPERATORS

void Oper_Movement (Operand &opd1, Operand &opd2);

class Oper_Mov : public BinaryOperator {
public:
    Oper_Mov() : BinaryOperator("mov") { oper = Oper_Movement;}
};


extern unary_instr_set uset; 
extern binary_instr_set bset;

extern InstrSet iset;


//  OPERATORS




/*class Op_Data : Operator{

    void operator() (ID lbl, int data, DataMemory& dm) {
        dm+={lbl.get_id(), data};
    }
};*/







