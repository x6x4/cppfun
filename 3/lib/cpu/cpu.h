#pragma once

#include "../qasm/mcode_compiler/mcode_compiler.h"
#include "../mem/mem.h"
#include <cstddef>
#include <stdexcept>
#include <utility>
#include <vector>



class CPU;


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  Contains general-purpose registers. 
 *  Registers can be: 
 *      accessed through number;
 *      blocked.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class Mem_Register;

class RegBlock {
friend Mem_Register;

    std::vector<int> regs;

    int operator[] (std::size_t num) const;
};


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Implement read-fetch-execute cycle. 
 * It fetches next instruction from program memory 
 * and then assigns it to free execution unit.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class ControlUnit {
friend CPU;
    
    void operator()(CPU &cpu);
    const Command &fetch(CPU &cpu) const;
    //  changes exec units state
    void assign(CPU &cpu, const Command &cmd);

};

using exectime_t = std::size_t; 
using instr_set = std::vector<std::pair<Operator, exectime_t>>;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Execute command and blocks its operands. 
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class ExecUnit {

friend ControlUnit;

    instr_set set;
    void exec (const Command &cmd) const;
    
};


using EU_vec = std::vector<std::pair<bool,ExecUnit>>;

using num_t = std::size_t;


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  The main device, contains all other devices. 
 *  It can execute program, transferring control to control unit.
 *  Its configuration can be changed. 
 *  
 *  Program memory:  |cmd1|cmd2|cmd3|cmd4|...|
 *  Control unit:    | cmd | -> EU
 *  Execution units: | cmd3 | | cmd1 | | free | | cmd2 |
 *  
 *  General-purpose register block:  |reg1|reg2|reg3|reg4|...|
 *  Data memory: |||||||||||||||||||||||||
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


class CPU {
friend ControlUnit;

    std::size_t bitness;
    
    ControlUnit cu;
    EU_vec EUs;
    RegBlock gp;
    Memory m;

    InstrSet iset;

    static bool CPU_exists;
    void check_existence(); 

public:

    CPU(InstrSet& _iset) : iset (_iset) { check_existence(); }
    /*CPU(EU_vec eu, RegBlock &gp, Memory &m);*/
    
    RegBlock &get_regblock () { return gp; }

    void exec (const MCode &mc);
    CPU &edit_config(); 
};

































    

