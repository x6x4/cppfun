#include "instr_set/instr_set.h"
#include "lib/IR_compiler/fwd_IR_compiler.h"

#include <iostream>

class CLI_DBG {

CPU *cpu = nullptr;

public:

    CLI_DBG (CPU *_cpu) : cpu(_cpu) {};

    void operator() (bpNum) { 
        cpu->print_gpregblock(std::cout); 
        //cpu->print_dm(std::cout); 
        cpu->print_spregblock(std::cout);
    };
};


int main (int, char **argv) {

    try {
        CPU cpu (iset, 10);

        if (!argv[1]) throw std::logic_error ("No arguments.");

        my_std::Vec<std::size_t> avl_bps;
        std::string program_text = ".text\nfsym %r6 z $key\n.data\nkey: .ascii k";
        for (int i = 0; i < 1000; i++)
            program_text.push_back('a');
        
        //load_file_cpu(cpu, argv[1], avl_bps);
        load_text_cpu(cpu, program_text, avl_bps);

        my_std::Vec <bpNum> bps = {bpNum(1, 0), bpNum(2, 0), bpNum(3, 0), bpNum(4, 0), bpNum(5, 0)};

        std::function<void(bpNum)> f = CLI_DBG(&cpu);
        
        exec(cpu, bps, f);
        std::cout << std::endl;
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


