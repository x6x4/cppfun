
#include "instr_set.h"


void Oper_Increment (Operand &opd1) {
    opd1.set(opd1.val() + 1);
};


void Oper_Movement (Operand &opd1, Operand &opd2) {
    opd1.set(opd2.val());
};



