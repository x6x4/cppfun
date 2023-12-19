
#include "gen/gen.h"
#include <iostream>


int main (int, char **argv) {

    try {

        InstrSet iset = load_iset();
        CPU cpu (iset, 25);

        if (!argv[1]) throw std::logic_error ("No arguments.");

        my_std::Vec<std::size_t> avl_bps;
        std::string program_text = ".text\nfsym %r6 z $key\n.data\nkey: .ascii k";
        for (int i = 0; i < 32000000; i++)
            program_text.push_back('a');

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


