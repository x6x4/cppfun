#include "instr_set/instr_set.h"
#include "lib/cpu/cpu.h"
#include "lib/qasm/cmd_base/cmd_base.h"
#include "lib/qasm/mcode_compiler/mcode_compiler.h"
#include <cstring>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>


int main (int argc, char **argv) {

    try {

        CPU cpu (iset);
        
        const MCode& mcode = file_to_mcode(iset, argv[1]);

        cpu.exec(mcode);

        std::cout << mcode;
    }

    catch (std::logic_error &e) {
        std::cout << e.what() << std::endl;
    }

    catch (std::runtime_error &e) {
        std::cout << e.what() << std::endl;
    }
    
}


