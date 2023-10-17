#include "instr_set/instr_set.h"
#include "lib/cpu/cpu.h"
#include "lib/qasm/cmd_base/cmd_base.h"
#include <cstring>
#include <functional>
#include <iostream>
#include <stdexcept>


int main (int argc, char **argv) {
    
    try {

        unary_instr_set uset ({Oper_Inc()}); 
        binary_instr_set bset ({Oper_Mov()});

        InstrSet iset {uset, bset};

        CPU cpu (iset);
        //CPU cpu1;

        const MCode *mcode = to_mcode(iset, argv[1]);
        std::cout << *mcode;

        //cpu.exec(mcode);
    }

    catch (std::logic_error &e) {
        std::cout << e.what() << std::endl;
    }

    catch (std::runtime_error &e) {
        std::cout << e.what() << std::endl;
    }
    
}


