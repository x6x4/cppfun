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


void hui() {
    std::cout << "Hui!" << std::endl;
}

int main (int argc, char **argv) {

    try {
        CPU cpu (iset);
        std::ifstream is (argv[1]);
        std::string line; 
        while (std::getline(is, line)) { 
            std::istringstream iss(line); 

        std::vector <std::size_t> bps = {1, 3};

        Mem mcode = file_to_mcode(iset, is, bps);
        fill (*mcode.second, hui);
        
        cpu.load_mem(std::move(mcode));
        
        cpu.exec();
    }

    catch (std::logic_error &e) {
        std::cout << e.what() << std::endl;
    }

    catch (std::runtime_error &e) {
        std::cout << e.what() << std::endl;
    }
    
}


