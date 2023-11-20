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
#include <type_traits>

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


int main (int, char **argv) {

    try {
        CPU cpu (iset);

        if (!argv[1]) throw std::logic_error ("No arguments.");
        
        load_file_cpu(cpu, argv[1]);

        my_std::Vec <std::size_t> bps = {1, 2, 3, 4, 5};

        // store a call to a function object
        std::function<void()> f = CLI_DBG(&cpu);
        
        exec(cpu, bps, f);
        std::cout << std::endl;

        /*load_file_cpu(cpu, argv[1]);
        exec(cpu, bps, f);
        std::cout << std::endl;*/
    }

    catch (std::logic_error &e) {
        std::cout << e.what() << std::endl;
    }

    catch (std::runtime_error &e) {
        std::cout << e.what() << std::endl;
    }

    catch (...) {
        std::cout << "Something went wrong" << std::endl;
    }

    std::cout << "quit" << std::endl;
    
}


