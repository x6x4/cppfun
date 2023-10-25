
#include "instr_set.h"
#include <stdexcept>


unary_instr_set uset ({Oper_Inc(), Oper_Jmp()}); 
binary_instr_set bset ({Oper_Mov()});

InstrSet iset {uset, bset};


//  UNARY OPERATORS

void Oper_Increment (Operand &opd1) {
    opd1.set(opd1.val() + 1);
};

void Oper_Jump (Operand &opd1) {
    if (typeid(opd1) == typeid(SPRegister))
        static_cast<SPRegister&>(opd1).set_num(0);
    else 
        throw std::logic_error ("Runtime error: jmp instruction requires code label operand");
}

//  BINARY OPERATORS

void Oper_Movement (Operand &opd1, Operand &opd2) {
    opd1.set(opd2.val());
};



