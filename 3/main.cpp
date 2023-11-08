#include "instr_set/instr_set.h"
#include "lib/IR_compiler/fwd_IR_compiler.h"
#include <cstddef>
#include <cstring>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <sstream>
#include <string>

class CLI_DBG {

CPU *cpu;

public:

    CLI_DBG (CPU *_cpu) : cpu(_cpu) {};

    void operator() () { 
        cpu->print_gpregblock(std::cout); 
        cpu->print_dm(std::cout); 
        cpu->print_spregblock(std::cout); 
    };
};


int main (int argc, char **argv) {

    try {
        CPU cpu (iset);
        
        load_file_cpu(cpu, "/home/cracky/cppfun/3/prog2.asm");

        std::vector <std::size_t> bps = {1, 4};

        // store a call to a function object
        std::function<void()> f = CLI_DBG(&cpu);
        
        exec(cpu, bps, f);

        load_file_cpu(cpu, "/home/cracky/cppfun/3/prog2.asm");
        exec(cpu, bps, f);
    }

    catch (std::logic_error &e) {
        std::cout << e.what() << std::endl;
    }

    catch (std::runtime_error &e) {
        std::cout << e.what() << std::endl;
    }
    
}


