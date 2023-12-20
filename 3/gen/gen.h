#pragma once
#include "../lib/IR_compiler/fwd_IR_compiler.h"
#include "../plugin/plugin_internals/library_manager/include/library_manager.h"

class CLI_DBG {

CPU *cpu = nullptr;

public:

    CLI_DBG (CPU *_cpu) : cpu(_cpu) {};

    void operator() (bpNum) { 
        cpu->print_gpregblock(std::cout); 
        cpu->print_dm(std::cout); 
        cpu->print_spregblock(std::cout);
    };
};

class CLI_DBG_MT {

CPU *cpu = nullptr;

public:

    CLI_DBG_MT (CPU *_cpu) : cpu(_cpu) {};

    void operator() (bpNum) { 
        cpu->print_gpregblock(std::cout); 
        cpu->print_spregblock(std::cout);
    };
};

InstrSet load_iset (LibraryManager &libManager);