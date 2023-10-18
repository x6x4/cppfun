#include "instr_set/instr_set.h"
#include "lib/cpu/cpu.h"
#include "lib/qasm/cmd_base/cmd_base.h"
#include "lib/qasm/mcode_compiler/mcode_compiler.h"
#include <cstring>
#include <functional>
#include <iostream>
#include <stdexcept>


int main (int argc, char **argv) {

    MCode mcode;
    
    try {

        CPU cpu (iset);
        //CPU cpu1;
        
        mcode = file_to_mcode(iset, argv[1]);

        std::cout << mcode;
        // << mcode;

        //mcode.~MCode();
        //delete mcode;

        //cpu.exec(mcode);
    }

    catch (std::logic_error &e) {
        std::cout << e.what() << std::endl;
        mcode.~MCode();
        //delete mcode;
    }

    catch (std::runtime_error &e) {
        std::cout << e.what() << std::endl;
        mcode.~MCode();
        //delete mcode;
    }
    
}


