#include "instr_set/instr_set.h"
#include "lib/IR_compiler/fwd_IR_compiler.h"



class CLI_DBG {

CPU *cpu;

public:

    CLI_DBG (CPU *_cpu) : cpu(_cpu) {};

    void operator() (bpNum) { 
        cpu->print_gpregblock(std::cout); 
        cpu->print_dm(std::cout); 
        cpu->print_spregblock(std::cout);
    };
};

#include <iostream>


int main (int, char **argv) {

    try {
        CPU cpu (iset);

        if (!argv[1]) throw std::logic_error ("No arguments.");

        my_std::Vec<std::size_t> avl_bps;
        
        load_file_cpu(cpu, argv[1], avl_bps);

        my_std::Vec <bpNum> bps = {bpNum(1, 0), bpNum(2, 0), bpNum(3, 0), bpNum(4, 0), bpNum(5, 0)};

        // store a call to a function object
        std::function<void(bpNum)> f = CLI_DBG(&cpu);
        
        exec(cpu, bps, f);
        std::cout << std::endl;
        //exec(cpu, bps, f);
        //std::cout << std::endl;

        /*load_file_cpu(cpu, argv[1]);
        */
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


