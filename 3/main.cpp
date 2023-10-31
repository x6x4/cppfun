#include "instr_set/instr_set.h"
#include "lib/IR_compiler/fwd_IR_compiler.h"
#include "lib/internals/cpu/cpu.h"
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

 
class PrintNum
{

CPU *cpu;

public:
    void operator()(int) const
    {
        
    }
};

void word() {
    std::cout << "Word!" << std::endl;
}

int main (int argc, char **argv) {

    try {
        CPU cpu (iset);
        //CLI cli (&cpu);
        std::ifstream is (argv[1]);

        std::unordered_set<ID> data_label_table;

        strings vec = preproc_text(is);
        std::unique_ptr<Data> data = parse_data(is, data_label_table);
        std::unique_ptr<SafeText> text = parse_text(iset, vec, data_label_table);

        Mem mcode = std::make_pair(std::move(data), std::move(text));
        cpu.load_mem(std::move(mcode));

        std::vector <std::size_t> bps = {1, 4};

        // store a call to a function object
        std::function<void()> f = CLI_DBG(&cpu);
        
        exec(cpu, bps, f);
    }

    catch (std::logic_error &e) {
        std::cout << e.what() << std::endl;
    }

    catch (std::runtime_error &e) {
        std::cout << e.what() << std::endl;
    }
    
}


