#include "instr_set/instr_set.h"
#include "lib/internals/cpu/cpu.h"
#include <cstring>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>


int main (int argc, char **argv) {

    try {
        CPU cpu (iset);
        cpu.exec(argv[1]);
        
    }

    catch (std::logic_error &e) {
        std::cout << e.what() << std::endl;
    }

    catch (std::runtime_error &e) {
        std::cout << e.what() << std::endl;
    }
    
}


