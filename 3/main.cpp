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


void word() {
    std::cout << "Word!" << std::endl;
}

int main (int argc, char **argv) {

    try {
        CPU cpu (iset);
        std::ifstream is (argv[1]);

        std::unordered_set<ID> data_label_table;

        strings vec = preproc_text(is);
        std::unique_ptr<Data> data = parse_data(is, data_label_table);
        std::unique_ptr<SafeText> text = parse_text(iset, vec, data_label_table);

        Mem mcode = std::make_pair(std::move(data), std::move(text));
        cpu.load_mem(std::move(mcode));

        std::vector <std::size_t> bps = {1, 4};
        
        cpu.exec(bps, word);
    }

    catch (std::logic_error &e) {
        std::cout << e.what() << std::endl;
    }

    catch (std::runtime_error &e) {
        std::cout << e.what() << std::endl;
    }
    
}


