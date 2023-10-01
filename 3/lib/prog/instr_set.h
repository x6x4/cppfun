#pragma once
#include "../mem/mem.h"
#include "prog.h"
#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  SYNTAX:  [label] op op1 [op2]
 *
 *  INSTRUCTION SET:
 *      bin_instr  op1, op2
            reg label
            reg reg
            reg imm_op

        un_instr op1
            reg
            label
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */



class Op_Jump : Operator {

    void operator() (addr_t addr, std::size_t &pc) const {
        pc = addr;
    };
};

class Op_Data : Operator{
    Operand num;

    void operator() (ID lbl, int data, DataMemory& dm) {
        dm+={lbl.get_id(), data};
    }
};

using mnemonic = std::string;
//std::vector<std::pair<Operator, mnemonic>> instr_set;

