#pragma once
#include "../mem/mem.h"
#include "prog.h"



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

    Op_Jmp() { Operator::mnem = "jmp"; }

    
};

class Op_Je : public Op_J {

void operator() (std::size_t code_lbl, ProgramMemory &pm) const {
    if (!get_zf(pm))
        Op_J::set_pc(code_lbl, pm);
};

public:

    Op_Je() { Operator::mnem = "je"; }


};

class Op_Jne : public Op_J {

void operator() (std::size_t code_lbl, ProgramMemory &pm) const {
    if (get_zf(pm))
        Op_J::set_pc(code_lbl, pm);
};

public:

    Op_Jne() { Operator::mnem = "jne"; }


};


//  UNARY OPERATORS

class Op_Inc : public Operator {

    void operator() (Operand &opd1) const {
        opd1.set(opd1.val() + 1);
    };

public:

    Op_Inc() { Operator::mnem = "je"; }
};

class Op_Neg : public Operator {

    void operator() (Operand &opd1) const {
        opd1.set(-opd1.val());
    };

public:

    Op_Neg() { Operator::mnem = "neg"; }
};

//  BINARY OPERATORS

class Op_Mov : public Operator {

    void operator() (Operand &opd1, Operand &opd2) const {
        opd1.set(opd2.val());
    };

public:

    Op_Mov() { Operator::mnem = "mov"; }
};

class Op_Cmp : public Op_J {

void operator() (Operand &opd1, Operand &opd2, ProgramMemory &pm) const {
    set_pc(opd1.val() != opd2.val(), pm);
};

public:
    Op_Cmp() { Operator::mnem = "cmp"; }

};



/*class Op_Data : Operator{

    void operator() (ID lbl, int data, DataMemory& dm) {
        dm+={lbl.get_id(), data};
    }
};*/




//using mnemonic = std::string;
//std::vector<std::pair<Operator, mnemonic>> instr_set;

