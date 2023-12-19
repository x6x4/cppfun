
#include "gen/gen.h"
#include <iostream>
#include <filesystem>
#include <memory>


int main (int, char **argv) {

    try {

        InstrSet *iset = load_iset();
        Operator test = iset->FindOper("inc");

        my_std::Vec<std::unique_ptr<Operand>> b;
        b.push_back({std::make_unique<Operand>(Operand())});

        test.oper(b);
        
        CPU cpu (*iset, 4);

        if (!argv[1]) throw std::logic_error ("No arguments.");

        my_std::Vec<std::size_t> avl_bps;
        
        load_file_cpu(cpu, argv[1], avl_bps);

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


