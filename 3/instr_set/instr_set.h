/** @file instr_set/instr_set.h
 *  This file contains user-defined unary and binary operators.
 */

#pragma once
#include "../lib/mem/mem.h"
#include <cstddef>


//  UNARY OPERATORS

void Oper_Increment (Operand &opd1);

class Oper_Inc : public UnaryOperator {

public:
    Oper_Inc() : UnaryOperator("inc") { oper = Oper_Increment; }
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

class Op_J : public Operator{

protected:
    void set_pc (std::size_t _pc, ProgramMemory &pm) const { pm.set_pc(_pc); }
    bool get_zf (ProgramMemory &pm) const { return pm.get_zf(); }

};

class Op_Jmp : public Op_J {

void operator() (std::size_t code_lbl, ProgramMemory &pm) const {
    Op_J::set_pc(code_lbl, pm);
};

public:
    
};

class Op_Je : public Op_J {

void operator() (std::size_t code_lbl, ProgramMemory &pm) const {
    if (!get_zf(pm))
        Op_J::set_pc(code_lbl, pm);
};

public:

};

class Op_Jne : public Op_J {

void operator() (std::size_t code_lbl, ProgramMemory &pm) const {
    if (get_zf(pm))
        Op_J::set_pc(code_lbl, pm);
};

public:

};

class Op_Neg : public Operator {

    void operator() (Operand &opd1) const {
        opd1.set(-opd1.val());
    };

public:

};

class Op_Cmp : public Op_J {

void operator() (Operand &opd1, Operand &opd2, ProgramMemory &pm) const {
    set_pc(opd1.val() != opd2.val(), pm);
};

public:

};


/*class Op_Data : Operator{

    void operator() (ID lbl, int data, DataMemory& dm) {
        dm+={lbl.get_id(), data};
    }
};*/







